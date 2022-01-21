#ifndef COMBINATION_H
#define COMBINATION_H

/*
 * Math functions that relate to making combinations and permutations of vectors of objects.
 */

#include <iostream>
#include <vector>

namespace common {
namespace tools {
namespace math {

// Make every combination of K elements from the vector passed.
// From https://hmkcode.com/calculate-find-all-possible-combinations-of-an-array-using-java/
template<typename T>
inline void kCombinations(int k, const std::vector<T> vec, std::vector<std::vector<T>>* combinations) {
  // get the length of the array
  // e.g. for {'A','B','C','D'} => N = 4
  int n = vec.size();

  if(k > n || k < 1){
    std::cout << " Invalid k and n " << k << " " << n << std::endl;
    return;
  }

  // init combination index array
  std::vector<int> pointers (k,0);

  int r = 0; // index for combination array
  int i = 0; // index for elements array

  while(r >= 0){

    // forward step if i < (N + (r-K))
    if(i <= (n + (r - k))){
      pointers[r] = i;

      // if combination array is full add the combinations to the tracked vector and increment i;
      if(r == k-1){
        std::vector<T> combinationOfElements;
        for (int index : pointers) {
          combinationOfElements.push_back(vec.at(index));
        }
        combinations->push_back(combinationOfElements);
        i++;
      }
      else{
        // if combination is not full yet, select next element
        i = pointers[r]+1;
        r++;
      }
    }
      // backward step
    else{
      r--;
      if(r >= 0)
        i = pointers[r]+1;
    }
  }

}

// Make every combination of K elements where k = 0, ... n - 1
template<typename T>
inline void allCombinations(const std::vector<T> vec, std::vector<std::vector<T>>* finalCombinations) {

  for (uint k = 0; k <= vec.size(); k++) {
    std::vector<std::vector<T>> currentKCombinations;
    kCombinations(k, vec, &currentKCombinations);

    // Merge the new combinations into all the combinations.
    std::move(std::begin(currentKCombinations),
              std::end(currentKCombinations), std::back_inserter(*finalCombinations));
  }
}

}  // math
}  // tools
}  // common

#endif // COMBINATION_H