 import sys
import requests

API_URL = "https://col-a7-api.jdhruv.com/get-keywords/"

def query(payload):
	response = requests.post(API_URL, json=payload)
	return response.json()

if __name__ == '__main__':

    # Read the paragraphs from the files

    with open("initial_query.txt", "r") as cfile:
        question = cfile.read()

    chat = query({'query': question})

    
    reply = chat['out']

    with open("keywords.txt", "w") as ofile:
        ofile.write(reply)