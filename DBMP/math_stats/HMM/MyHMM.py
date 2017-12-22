from typing import List, Dict
import numpy as np
import pandas as pd

"""
The task is:
    Construct a hidden Markov model for Set # 4.
    Construct a graph of the dependence
    probability from the iteration of the algorithm.
    

Problem solved with Baum–Welch algorithm;

"""


class MyLittleHMM:
    '''
    > nstates: the number of hidden states.
     we omit the initial and final states
     and assume an equal probability of starting from any state.

     > emission: numpy-matrix of size (N, |V|) (where |V| - the size of the vocabulary
                                      (or the number of unique values in observations)),
     emission (j, o) is the probability to emit the observation o from the state j;

     > transition: numpy-matrix of the form (N, N),
     where transition(i, j) gives the probability of transition from state i to state j.

     > forward & backward: numpy matrices of the form (N, T), where forward (s, t) [backward (s, t)]
     sets the "forward" ["backward"] probability of the observation t in the state s;

    '''

    def __init__(self, numstates: int, observations_vocab: Dict[str, int], verbose=True):

        self.verbose = verbose
        self.nstates = numstates
        self.vocabsize = len(observations_vocab)
        self.observations_vocab = observations_vocab

        self.transition = self._random_mtx_init(numstates, numstates)
        self.emission = self._random_mtx_init(numstates, self.vocabsize)
        self.init_probs = self._random_mtx_init(1, numstates)

        if self.verbose:
            print("INITIAL:\n")
            self.print_all()

    @staticmethod
    def _random_mtx_init(nrows, ncols) -> np.ndarray:
        """
        :param nrows:
        :param ncols:
        :return: just return random matrix with sum of each row = 1
                (stochastic matrices)
        """
        if nrows == 1:
            tmp = np.random.rand(ncols)
            return tmp / sum(tmp)
        else:
            mtx = np.zeros((nrows, ncols))
            for i in range(nrows):
                unnormalized = np.random.rand(ncols)
                mtx[i] = unnormalized / sum(unnormalized)
            return mtx

    def print_all(self):
        print("TRANSITION MTX:")
        print(self.transition)
        print("\nEMISSION MTX:")
        print(self.emission)
        print("\nINITIAL_PROBS:")
        print(self.init_probs)
        print('\n\n')

    def forward(self, observations: List[str]) -> np.ndarray:
        """
        :param observations:
        :return: matrix of size [NxT]

        Calculate forward probabilities:
            alpha_t (j) = P (o1, ..., ot, qt = j | HMM)

        """

        forwardmatrix = np.zeros((self.nstates, len(observations)))

        # initialization
        obs_index = self.observations_vocab[observations[0]]
        for s in range(self.nstates):
            forwardmatrix[s, 0] = self.init_probs[s] * self.emission[s, obs_index]

        # dynamically calculate probs for each time stamp
        for t in range(1, len(observations)):
            obs_index = self.observations_vocab[observations[t]]
            for s in range(self.nstates):
                forwardmatrix[s, t] = self.emission[s, obs_index] * sum(
                    [forwardmatrix[s2, t - 1] * self.transition[s2, s] \
                     for s2 in range(self.nstates)])
        return forwardmatrix

    def backward(self, observations: List[str]) -> np.ndarray:
        """
        :param observations:
        :return: matrix of size [NxT]

        """
        backwardmatrix = np.zeros((self.nstates, len(observations)))

        # initialization
        for s in range(self.nstates):
            backwardmatrix[s, len(observations) - 1] = 1.0

        # dynamically calculate probs for each time stamp
        for t in range(len(observations) - 2, -1, -1):
            obs_index = self.observations_vocab[observations[t + 1]]
            for s in range(self.nstates):
                backwardmatrix[s, t] = sum([self.transition[s, s2] * self.emission[s2, obs_index] *
                                            backwardmatrix[s2, t + 1] for s2 in range(self.nstates)])

        return backwardmatrix

    def expectation(self, observations: List, forward_probs: np.ndarray, backward_probs: np.ndarray):
        """
        :param observations:
        :param forward_probs:
        :param backward_probs:
        :return: gamma, xi:
            - gamma is a (N, T) numpy matrix
            - xi is a list of T numpy matrices of size (N, N)

        expectation step:
        re-estimate xi_t(i, j) and gamma_t(j)

        """
        # denominator: P(O | HMM)
        p_o_given_hmm = sum([forward_probs[s_i, len(observations) - 1] for s_i in range(self.nstates)])

        # computing xi
        xi = []
        for t in range(len(observations) - 1):
            obs_index = self.observations_vocab[observations[t + 1]]

            xi_t = np.zeros((self.nstates, self.nstates))

            for s_i in range(self.nstates):
                for s_j in range(self.nstates):
                    xi_t[s_i, s_j] = (forward_probs[s_i, t] * self.transition[s_i, s_j] * self.emission[s_j, obs_index]
                                      * backward_probs[s_j, t + 1]) / p_o_given_hmm
            xi.append(xi_t)

        # computing gamma
        gamma = np.zeros((self.nstates + 2, len(observations)))
        for t in range(len(observations) - 1):
            for s_i in range(self.nstates):
                gamma[s_i, t] = sum([xi[t][s_i, s_j] for s_j in range(self.nstates)])

        for s_j in range(self.nstates):
            gamma[s_j, len(observations) - 1] = sum([xi[t][s_i, s_j] for s_i in range(self.nstates)])

        return gamma, xi

    def maximization(self, observations: List, gamma: np.ndarray, xi: List):
        """
        :param observations:
        :param gamma:
        :param xi:
        :return:
            - init_probs
            - transition
            - emission

        maximization step:
        re-estimate transition, emission based on gamma, xi

        """
        # re-estimate initial probabilities
        initialprob = np.array([gamma[s_i, 0] for s_i in range(self.nstates)])

        # re-estimate emission probabilities
        emis = np.zeros((self.nstates, self.vocabsize))

        for s in range(self.nstates):
            denominator = sum([gamma[s, t] for t in range(len(observations))])
            # because of numerical overloads
            for vocab_item, obs_index in self.observations_vocab.items():
                emis[s, obs_index] = sum(
                    [gamma[s, t] for t in range(len(observations)) if observations[t] == vocab_item]) / denominator

        # re-estimate transition probabilities
        trans = np.zeros((self.nstates, self.nstates))

        for s_i in range(self.nstates):
            denominator = sum([gamma[s_i, t] for t in range(len(observations) - 1)])
            for s_j in range(self.nstates):
                trans[s_i, s_j] = sum([xi[t][s_i, s_j] for t in range(len(observations) - 1)]) / denominator

        return initialprob, trans, emis

    def compute_one_step(self, observations: List):
        """
        :param observations:
        :return: gamma
                 which is a (N, T) numpy matrix

        Make one iteration step;

        """
        fwd = self.forward(observations)
        bwd = self.backward(observations)
        gamma, xi = self.expectation(observations, fwd, bwd)
        init_probs, transition, emission = self.maximization(observations, gamma=gamma, xi=xi)
        self.init_probs = init_probs
        self.transition = transition
        self.emission = emission

        if self.verbose:
            self.print_all()
        return emission, transition, init_probs

    def compute_several_steps(self, observations: List, num_steps: int):
        """
        :param observations: List of observations
        :param num_steps: number of iteration steps
        :return:
         - emission_list,
         - transition_list,
         - init_probs_list

        length of each list = num_steps;

        Compute num_steps iterations of Baum–Welch algorithm to obtain
        emission, transition and initial probabilities after such num_steps;

        """
        emis_list = []
        trans_list = []
        init_probs_list = []

        for i in range(num_steps):
            if self.verbose:
                print("\n--------------->> STEP#{} <<-------------------\n".format(i+1))
            emission, transition, init_probs = self.compute_one_step(observations=observations)
            emis_list.append(emission)
            trans_list.append(transition)
            init_probs_list.append(init_probs)

        return emis_list, trans_list, init_probs_list


if __name__ == '__main__':
    data = pd.read_csv("./set4.txt", sep=' ', index_col=0, header=None)[1].values
    obs_vocab = {'L': 0, 'R': 1}
    numstates = 2
    iteration_num = 5
    hmm = MyLittleHMM(numstates=numstates, observations_vocab=obs_vocab)
    emis_list, trans_list, init_probs_list = hmm.compute_several_steps(observations=data, num_steps=iteration_num)
