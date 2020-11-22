def stich(original: list, new_list: list):

    max_overlap = (len(list1) > len(list2)) * len(list2) + (len(list2) > len(list1)) * len(list1)

    overlap = get_overlap(original, new_list, max_overlap)

    return original[:-overlap]+new_list


def get_overlap(list1: list, list2: list, max_overlap: int):

    for overlap in range(1, max_overlap):
        if check_overlap(list1, list2, overlap):
            return overlap

    return 0



def check_overlap(list1, list2, overlap):

    if overlap > len(list1) or overlap > len(list2):
        return False

    for i in range(overlap):
        if not list1[i-overlap] == list2[i]:
            return False

    return True
