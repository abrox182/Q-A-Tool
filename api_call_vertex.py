import sys
import requests

API_URL = "https://col-a7-api.jdhruv.com/vertex-ai/"

def query(payload):
	response = requests.post(API_URL, json=payload)
	return response.json()

if __name__ == '__main__':

    # Read the paragraphs from the files

    with open("question.txt", "r") as cfile:
        question = cfile.read()
    with open("paragraphs.txt", "r") as cfile:
        paras = cfile.read()

    print("Querying vertex")
    chat = query({'query': question, 'context': paras})

    reply = chat['out']
    print(reply)

    with open(sys.argv[1], "w") as ofile:
        ofile.write(reply)