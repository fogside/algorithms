"""
Дана матрица А, размера MxN,
заполненная произвольными действительными числами.
Необходимо написать программу, которая находит в этой матрице
квадратную подматрицу с максимальной суммой элементов.

"""

import numpy as np

def max_subarray_kadane(arr):
    max_from = max_from_ending_here = max_to = 0
    max_so_far = max_ending_here = arr[0]
    for n, x in enumerate(arr[1:]):
        if x > max_ending_here + x:
            max_from_ending_here = n + 1
            max_ending_here = x
        else:
            max_ending_here += x
        if max_ending_here > max_so_far:
            max_so_far = max_ending_here
            max_from = max_from_ending_here
            max_to = n + 1
    return max_so_far, max_from, max_to


def max_submatrix(m):
    m = np.array(m)
    h, w = m.shape
    max_sum, max_up, max_down = max_subarray_kadane(m[0])
    max_left = max_right = 0
    for l in range(w):
        tmp_arr = np.zeros(h)
        for r in range(l, w):
            tmp_arr += m[:, r]
            tmp_sum, tmp_up, tmp_down = max_subarray_kadane(tmp_arr)
            if tmp_sum > max_sum:
                max_sum = tmp_sum
                max_up, max_down, max_left, max_right = tmp_up, tmp_down, l, r
    print("Resulted submatrix with maximum sum:")
    print(m[max_up: max_down + 1, max_left: max_right + 1])
    return max_sum, max_up, max_down, max_left, max_right

if __name__ == '__main__':
    m1 = [[2, 1, -3, -4, 5],
         [0, 6,  3,  4, 1],
         [2, -2, -1, 4, -5],
         [-3, 3,  1,  0, 3]]

    m2 = [[1, 2, -1, -4, -20],
         [-8, -3, 4, 2, 1],
         [3, 8, 10, 1, 3],
         [-4, -1, 1, 7, -6]]

    k1 = max_submatrix(m1)
    k2 = max_submatrix(m2)

    assert k1[0] == 18
    assert k2[0] == 29