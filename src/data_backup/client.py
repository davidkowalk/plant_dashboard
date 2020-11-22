from tkinter import filedialog, Tk
import requests, json

def stich(original: list, new_list: list):

    max_overlap = (len(original) > len(new_list)) * len(new_list) + (len(new_list) > len(original)) * len(original)

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

def add_data(path):

    with open(path, "r") as f:
        txt = f.read()

    try:
        old = json.loads(txt)
    except:
        print("Failed to read json.")
        print(txt)
        return

    # Get New Data
    ip = "http://192.168.178.64/data"
    response = requests.get(ip)

    if response.status_code != 200:
        print(f"Connection Failed! ({response.status_code})")
        print(response.headers)
        return

    new = response.json()

    # Find beginning of data
    start = 0
    for i in range(len(new)):
        start = i

        if new[i] != 0:
            break

    new = new[start:]

    # Stich old and new data together
    stiched = stich(old, new)

    # Save Data

    with open(path, "w") as f:
        json.dump(stiched, f, indent=2)

def main():

    root = Tk()
    root.withdraw()
    path = filedialog.askopenfilename()

    if path == "":
        print("No File")
        return

    add_data(path)

if __name__ == '__main__':
    add_data()
