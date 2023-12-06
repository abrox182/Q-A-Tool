import openai
import sys

openai.api_key = "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"

if __name__ == '__main__':
    
    # Read the paragraphs from the files
    paragraphs = ["Background: This is from Gandhi's memoirs written in both first and third person: "]

    filename = 'paragraphs.txt'
    with open(filename, 'r') as f:
        paragraphs.append(f.read())
        paragraphs.append('\n\n\n')
    
    # add query
    with open('question.txt', 'r') as f:
        query = f.read()
        paragraphs.append("\nWithin the context given above, do your best to answer the following question: ")
        paragraphs.append(query)
        paragraphs.append('\n')

    # convert paragraphs to a single string
    paragraphs = '\n'.join(paragraphs)

    print(paragraphs)

    query = {
        "role": "user", "content": paragraphs
    }

    chat = openai.ChatCompletion.create(
        model="gpt-3.5-turbo",
        messages=[query]
    )

    reply = chat.choices[0].message.content
    print(reply)

    with open(sys.argv[1], "w") as ofile:
        ofile.write(reply)