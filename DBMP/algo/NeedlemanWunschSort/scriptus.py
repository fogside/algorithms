import pandas as pd
import numpy as np

# for unix systems execute:
#    easy_install -f http://biopython.org/DIST/ biopython
from Bio import SeqIO

'''
31.10 Темы: "Бинарные деревья", "Динамическое программирование в решении задач выравнивания последовательностей".
4.
На вход программе подаётся референсная строка и fasta-файл, в котором содержатся риды.
Необходимо отсортировать риды по величине оценки выравнивания с помощью двоичного дерева.
В качестве матрицы замен использовать следующую матрицу:

Штрафы за indels = -0.5

Уточняю формат вывода в задаче 4: на экран или в файл выводятся результаты выравнивания ридов в порядке
убывания величины оценки выравнивания.
Замены обозначаются нижним регистром символов, вставки и делеции - дефисом в референсе или риде.

Пример:
Референс: ATTTGGTTC
Рид:      ATCTGTTC

Результат выравнивания:
ATTTGGTTC
ATcTG-TTC

'''
substitution_matrix = pd.DataFrame(
    data=[[1, 0.5, 0.1, 0.1],
          [0.5, 1, 0.1, 0.1],
          [0.1, 0.1, 1, 0.5],
          [0.1, 0.1, 0.5, 1]],
    columns=np.array(list("A  T  G  C".replace("  ", ""))),
    index=np.array(list("A  T  G  C".replace("  ", ""))))


def constructAlignmentMatrix(ref, seq, substitution_matrix, penalty):
    """
    :param ref:
    :param seq:
    :param substitution_matrix:
    :param penalty:
    :return:
     returns a global alignment penalty,
      accepting alignment objects (ref,seq), a substitution_matrix, and a pass penalty
    """
    S = [[0 for j in range(len(ref) + 1)] for i in range(len(seq) + 1)]

    for i in range(1, len(seq) + 1):
        S[i][0] = -i * penalty
    for j in range(1, len(ref) + 1):
        S[0][j] = -j * penalty

    for i in range(1, len(seq) + 1):
        for j in range(1, len(ref) + 1):
            scores = [S[i - 1][j] - penalty, S[i][j - 1] - penalty,
                      S[i - 1][j - 1] + substitution_matrix[seq[i - 1]][ref[j - 1]]]
            S[i][j] = max(scores)

    return S


def NeedlemanWunschAlignment(seq1, seq2, F, penalty, sub_matrix):
    """
    :param seq1: list of letters
    :param seq2: list of letters
    :param F: matrix penalties
    :param penalty:
    :param sub_matrix:
    :return:

    """
    AlignmentA = ""
    AlignmentB = ""
    i = len(seq1)
    j = len(seq2)

    # right bottom corner
    while ((i > 0) or (j > 0)):
        if ((i > 0) and (j > 0) and F[i][j] == F[i - 1][j - 1] + sub_matrix[seq1[i - 1]][seq2[j - 1]]):

            if (seq1[i - 1] == seq2[j - 1]):
                # match
                AlignmentA = seq1[i - 1] + AlignmentA
                AlignmentB = seq2[j - 1] + AlignmentB
            else:
                # or not
                AlignmentA = seq1[i - 1].lower() + AlignmentA
                AlignmentB = seq2[j - 1] + AlignmentB
            i = i - 1
            j = j - 1

        elif ((i > 0) and F[i][j] == F[i - 1][j] + penalty):
            # insertion
            AlignmentA = seq1[i - 1] + AlignmentA
            AlignmentB = "-" + AlignmentB
            i = i - 1
        else:
            # deletion
            AlignmentA = "-" + AlignmentA
            AlignmentB = seq2[j - 1] + AlignmentB
            j = j - 1

    return (AlignmentA, AlignmentB)


if __name__ == '__main__':

    REF_PATH = '/home/fogside/Projects/algorithms/DBMP/algo/NeedlemanWunschSort/ref.txt'
    READS_PATH = '/home/fogside/Projects/algorithms/DBMP/algo/NeedlemanWunschSort/reads.txt'

    # it is expected that ref is one and only one;
    with open(REF_PATH, 'r') as fref:
        ref = fref.readline().strip()
    ref_l = list(ref)

    with open(READS_PATH) as freads:
        data = list(SeqIO.parse(freads, 'fasta'))

    alignments = {}
    for record in data:
        seq = str(record.seq)
        seq_l = list(seq)
        F = constructAlignmentMatrix(ref, seq, substitution_matrix, 0.5)
        alignments[NeedlemanWunschAlignment(seq_l, ref_l, F, 0.5, substitution_matrix)[0]] = F[len(seq_l)][len(ref_l)]

    sorted_al = sorted(alignments.items(), key=lambda x: x[1])
    for i in range(0, len(sorted_al)):
        print("score:", sorted_al[i][1], "\n",
              "ref:", ref, "\n",
              "res:", sorted_al[i][0])
        print("-------------\n")

"""
Sample output for given ref & reads:

score: 3.5 
 ref: ATATAGTAATG 
 res: --ATA-T-A-G
-------------

score: 5.0 
 ref: ATATAGTAATG 
 res: ATATAG--A--
-------------

score: 5.6 
 ref: ATATAGTAATG 
 res: ATATAG---cG
 
-------------

"""