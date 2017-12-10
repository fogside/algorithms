#include <iostream>
#include <vector>
#include <stack>
#include <cassert>

/*
 * Дана матрица A, размера MxN, заполненная в произвольном порядке нулями и единицами.
 * Необходимо написать программу, которая находит в этой матрице
 * наибольшую прямоугольную подматрицу, заполненную единицами.
 */

using namespace std;


int maxHistRectangleArea(vector<int> &row, int &height) {
    // first argument is for input raw histogram;
    // second argument is for saving coordinates of current max_area;

    stack<int> bars; //stack with indexes;
    // of bars according to their
    // increasing value order;
    int top;
    int area;
    int max_hist_area = 0;

    int i = 0;
    while (i < row.size()) {
        if (bars.empty() || row[bars.top()] <= row[i]) {
            bars.push(i);
            ++i;

        } else if (row[bars.top()] > row[i]) {
            top = bars.top();
            bars.pop();
            if (!bars.empty())
                area = row[top] * (i - top);
            else
                area = i * row[top];

            max_hist_area = max(max_hist_area, area);
        }
    }

    // update current max_hist_area with left bars;
    while (!bars.empty()) {
        top = bars.top();
        bars.pop();
        if (!bars.empty())
            area = row[top] * (i - top);
        else
            area = i * row[top];

        max_hist_area = max(max_hist_area, area);
    }

    return max_hist_area;
}

int maxRectangleArea(vector<vector<int> > &input) {

    int max_area = 0;
    int ncols = input[0].size();
    int nrows = input.size();
    int height = 0;

    for (int i = 0; i < nrows; ++i) {
        //building histogram for that particular raw:
        if (i > 0)
            for (int j = 0; j < ncols; ++j) {
                if (input[i][j])
                    input[i][j] += input[i - 1][j] ? input[i - 1][j] : 0;
            }
        //update maximum area:
        max_area = max(max_area, maxHistRectangleArea(input[i], height));
    }

//    int width = max_area / height;
//    for (int i = 0; i < height; ++i) {
//        for (int j = 0; j < width; ++j)
//            cout << 1 << ' ';
//        cout << '\n';
//    }

    return max_area;
}

int main() {
    vector<vector<int>> exmpl_mtx1 ={{0, 1, 1, 0},
                                     {1, 1, 1, 1},
                                     {1, 1, 1, 1},
                                     {1, 1, 0, 0}};

    vector<vector<int>> exmpl_mtx2 ={{1, 1, 1, 0},
                                     {1, 1, 1, 1},
                                     {1, 1, 1, 1},
                                     {1, 1, 0, 0}};

    vector<vector<int>> exmpl_mtx3 ={{0, 0, 1, 0},
                                     {1, 1, 0, 1},
                                     {0, 1, 0, 1},
                                     {1, 0, 0, 0}};

    vector<vector<int>> exmpl_mtx4 ={{1, 0, 1, 0},
                                     {0, 1, 0, 1},
                                     {1, 0, 1, 0},
                                     {0, 1, 0, 1}};

    vector<vector<int>> exmpl_mtx5 ={{1, 1, 1, 0},
                                     {1, 1, 0, 1}};

    assert(8 == maxRectangleArea(exmpl_mtx1));
    assert(9 == maxRectangleArea(exmpl_mtx2));
    assert(2 == maxRectangleArea(exmpl_mtx3));
    assert(1 == maxRectangleArea(exmpl_mtx4));
    assert(4 == maxRectangleArea(exmpl_mtx5));

    return 0;
}