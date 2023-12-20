def quicksort(arr, left, right):
    if left < right:
        sep = partition(arr, left, right)
        quicksort(arr, left, sep)
        quicksort(arr, sep + 1, right)


def partition(arr, left, right):
    sep = arr[(left + right) // 2]
    i, j = left, right
    while i <= j:
        while arr[i] < sep:
            i += 1
        while arr[j] > sep:
            j -= 1
        if i >= j:
            break
        arr[i], arr[j] = arr[j], arr[i]
        i += 1
        j -= 1
    return j

N = 4
arrs = []
for i in range(N):
    f = open(f'input{i}.txt')
    arr = [int(n) for n in f.read().split(' ')]
    quicksort(arr, 0, len(arr) - 1)
    arrs.append(arr)
    f.close()
sorted_arr = []
pointers = [0] * N
min_arr = 0
while min_arr != -1:
    min_element = 1e20
    min_arr = -1
    for i in range(N):
        if pointers[i] < len(arrs[i]) and arrs[i][pointers[i]] < min_element:
            min_element = arrs[i][pointers[i]]
            min_arr = i
    if min_arr != -1:
        pointers[min_arr] += 1
        sorted_arr.append(min_element)
f = open('output.txt', 'w')
f.write(str(sorted_arr))
f.close()