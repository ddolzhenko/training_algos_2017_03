
def merge(u, v):
    bu, eu = 0, len(u)
    bv, ev = 0, len(v)
    result = []
    while bu < eu and bv < ev:
        if u[bu] < v[bv]:
            result.append(u[bu])
            bu += 1
        else:
            result.append(v[bv])
            bv += 1
    result += u[bu:]
    result += v[bv:]
    assert(len(result) == len(u) + len(v))

    return result

def merge_sort(v):
    if(len(v) < 2):
        return v

    m = len(v) // 2
    left =  merge_sort(v[:m])
    right = merge_sort(v[m:])
    return merge(left, right)


v = [9,6,7,1,8]

print(merge_sort(v))

