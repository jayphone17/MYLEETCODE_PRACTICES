# 方法一：

def divide(self, a: int, b: int) -> int:
  INT_MIN, INT_MAX = -2 ** 31, 2 ** 31 - 1
  if a == INT_MIN and b == -1:
    return INT_MAX

  sign = -1 if (a > 0) ^ (b > 0) else 1

  a, b = abs(a), abs(b)
  ans = 0
  for i in range(31, -1, -1):
    if (a >> i) - b >= 0:
      a = a - (b << i)
      ans += 1 << i

  # bug 修复：因为不能使用乘号，所以将乘号换成三目运算符
  return ans if sign == 1 else -ans
if __name__ == '__main__':
  print(divide(0,99999999999999,127371298371))

#方法二：二分查找
class Solution:
  def divide(self, dividend: int, divisor: int) -> int:
    INT_MIN, INT_MAX = -2 ** 31, 2 ** 31 - 1

    # 考虑被除数为最小值的情况
    if dividend == INT_MIN:
      if divisor == 1:
        return INT_MIN
      if divisor == -1:
        return INT_MAX

    # 考虑除数为最小值的情况
    if divisor == INT_MIN:
      return 1 if dividend == INT_MIN else 0
    # 考虑被除数为 0 的情况
    if dividend == 0:
      return 0

    # 一般情况，使用二分查找
    # 将所有的正数取相反数，这样就只需要考虑一种情况
    rev = False
    if dividend > 0:
      dividend = -dividend
      rev = not rev
    if divisor > 0:
      divisor = -divisor
      rev = not rev

    # 快速乘
    def quickAdd(y: int, z: int, x: int) -> bool:
      # x 和 y 是负数，z 是正数
      # 需要判断 z * y >= x 是否成立
      result, add = 0, y
      while z > 0:
        if (z & 1) == 1:
          # 需要保证 result + add >= x
          if result < x - add:
            return False
          result += add
        if z != 1:
          # 需要保证 add + add >= x
          if add < x - add:
            return False
          add += add
        # 不能使用除法
        z >>= 1
      return True

    left, right, ans = 1, INT_MAX, 0
    while left <= right:
      # 注意溢出，并且不能使用除法
      mid = left + ((right - left) >> 1)
      check = quickAdd(divisor, mid, dividend)
      if check:
        ans = mid
        # 注意溢出
        if mid == INT_MAX:
          break
        left = mid + 1
      else:
        right = mid - 1

    return -ans if rev else ans

# 方法三：类二分查找
  class Solution:
    def divide(self, dividend: int, divisor: int) -> int:
      INT_MIN, INT_MAX = -2 ** 31, 2 ** 31 - 1

      # 考虑被除数为最小值的情况
      if dividend == INT_MIN:
        if divisor == 1:
          return INT_MIN
        if divisor == -1:
          return INT_MAX

      # 考虑除数为最小值的情况
      if divisor == INT_MIN:
        return 1 if dividend == INT_MIN else 0
      # 考虑被除数为 0 的情况
      if dividend == 0:
        return 0

      # 一般情况，使用类二分查找
      # 将所有的正数取相反数，这样就只需要考虑一种情况
      rev = False
      if dividend > 0:
        dividend = -dividend
        rev = not rev
      if divisor > 0:
        divisor = -divisor
        rev = not rev

      candidates = [divisor]
      # 注意溢出
      while candidates[-1] >= dividend - candidates[-1]:
        candidates.append(candidates[-1] + candidates[-1])

      ans = 0
      for i in range(len(candidates) - 1, -1, -1):
        if candidates[i] >= dividend:
          ans += (1 << i)
          dividend -= candidates[i]

      return -ans if rev else ans
