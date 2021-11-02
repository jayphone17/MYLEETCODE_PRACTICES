# 方法一：
# karatsuba 算法
# 时间复杂度为 O (n ^ log 3)

from math import log2, ceil

def pad(string: str, real_len: int, max_len: int) -> str:
  pad_len: int = max_len - real_len
  return f"{'0' * pad_len}{string}"


def kara(n1: int, n2: int) -> int:
  if n1 < 10 or n2 < 10:
    return n1 * n2
  n1_str: str = str(n1)
  n2_str: str = str(n2)
  n1_len: int = len(n1_str)
  n2_len: int = len(n2_str)
  real_len: int = max(n1_len, n2_len)
  max_len: int = 2 ** ceil(log2(real_len))
  mid_len: int = max_len >> 1
  n1_pad: str = pad(n1_str, n1_len, max_len)
  n2_pad: str = pad(n2_str, n2_len, max_len)
  p: int = int(n1_pad[:mid_len])
  q: int = int(n1_pad[mid_len:])
  r: int = int(n2_pad[:mid_len])
  s: int = int(n2_pad[mid_len:])
  u: int = kara(p, r)
  v: int = kara(q - p, r - s)
  w: int = kara(q, s)
  return u * 10 ** max_len + (u + v + w) * 10 ** mid_len + w

big_num = kara(123456789,987654321)
print(big_num)

# 方法二：
def recursive_multiply(x, y, n):
  if n == 1:
    return x * y

  else:
    a = x / pow(10, n / 2)
    b = x - a * pow(10, n / 2)
    c = y / pow(10, n / 2)
    d = y - c * pow(10, n / 2)

    ac = recursive_multiply(a, c, n / 2)
    bd = recursive_multiply(b, d, n / 2)
    p = recursive_multiply(a + b, c + d, n / 2) - ac - bd

    return ac * pow(10, n) + p * pow(10, n / 2) + bd

if __name__ == '__main__':
  r = recursive_multiply(1234, 5678, 4)
  print(r)

#方法三：
