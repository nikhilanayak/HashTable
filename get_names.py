import requests
from tqdm import tqdm

url = "https://randomuser.me/api/"
NUM_PERSONS = 100


fnames = []
lnames = []

ffile = open("FIRST_NAMES.txt", "w", encoding="utf-8")
lfile = open("LAST_NAMES.txt", "w", encoding="utf-8")


for i in tqdm(range(NUM_PERSONS)):
    res = requests.get(url).json()["results"][0]["name"]
    ffile.write(res["first"] + "\n")
    lfile.write(res["last"] + "\n")
