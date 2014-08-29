# **README for Insert Sort**
***


## **Summary**
  **采用in-place实现**

  * 从第一个元素开始, 该元素可认为已经排序
  * 取出下一个元素, 在已经排序的元素序列中从后向前扫描
  * 如果该元素(已排序)大于新元素, 将该元素移到下一位置
  * 重复步骤3, 直到找到已排序的元素小于或等于新元素的位置
  * 将新元素插入到该位置后
  * 重复2~5