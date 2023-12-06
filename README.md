# COL106 Assignment 7: Document Search Tool!

Hi! This is Team Caffeine (aka Team Abhi toh Dassi)'s submission for A7

## Our API Deployment

The most significant part of this project is: **to use only FOSS LLMs and to get around the limitations of free APIs, we decided to make our own.** This section will detail how to set up and deploy this API.

### Cloud provider
We used Google Cloud; similarly to ChatGPT, it offers a 300$ credit with a free new account; more than enough to finish this assignment. Sign up for a free GCloud account [here](https://cloud.google.com/).

### Set up a VM

Go to your Dashboard on GCloud and create a new VM instance with desired specs: for running inference, we wanted a GPU, and so went for `g2-standard-8` VM with `1x NVIDIA L4` GPU (for fas fas inference) and `c0-deeplearning-common-gpu-v20231105-debian-11-py310` Linux image; this image comes with CUDA preinstalled, saving a huge headache later on. This machine is in the `asia-south1-a` (Mumbai) zone.

### Set up software

Go to [Google Vertex AI](https://console.cloud.google.com/vertex-ai), activate it. Then, SSH into your shiny new VM and run `gcloud auth application-default login`. This will let you use Google's Vertex AI PaLM model; one of the best non-GPT generative models around.

We assume you're familiar with Linux's [screen command](https://linux.die.net/man/1/screen): the next steps will use it gratuitously.

Run `git clone https://github.com/jdhruv1503/col106-a7` to clone this repo into the home dir. `cd` into `col106-a7/Part 2 API`. Now, open a new `screen` and run `pip install -r requirements.txt` followed by `python ./main.py`. This will start our custom API server (written in FastAPI). As it keeps the screen captive and we need to do some other stuff, use `Ctrl+A,D` to exit this screen session.

### Networking
We used [Cloudflare Tunnels](https://www.cloudflare.com/en-gb/) to expose the API to the web; [ngrok](https://ngrok.com/) can be used as well but we don't prefer it due to CF Tunnels running neatly as a daemon and supporting custom domains for free. To set up networking:

1. Get a domain name ([Freenom](https://www.freenom.com/en/index.html?lang=en) offers free ones)
2. Transfer the domain to Cloudflare
3. Once nameservers are set up, go to Access on the left. This opens the Cloudflare Zero Trust dashboard. Go to Tunnels on the left.
4. Make a new tunnel. Follow the instructions to install the daemon to your GCloud system and configure the endpoint to point to http://localhost:8000/. (psst, also make one for port 8001 later, we'll need it :)) Here are the settings we used:

![enter image description here](https://i.postimg.cc/wTqJ5XnY/image.png)
## The HuggingFace model deployment

The free and open-source model used is the Vicuna model by FastChat ([GitHub](https://github.com/lm-sys/FastChat/tree/main)). They offer a OpenAI API-compatible REST API for their open-source model, [hosted on HuggingFace](https://huggingface.co/lmsys/vicuna-7b-v1.5).

This model is essentially a fine-tuned Facebook LLaMA model with 7 billion parameters.

In order to deploy, it would be best to refer to their docs; but, briefly, the way to deploy is to run the following commands on **separate screen sessions** on GCloud:

`python3 -m fastchat.serve.controller`	
`python3 -m fastchat.serve.model_worker --model-path lmsys/vicuna-7b-v1.5`
`python3 -m fastchat.serve.openai_api_server --host localhost --port 8001`

This will download the HF model automagically and will also serve a excellent FastAPI endpoint on port 8001 (which we exposed on Cloudflare earlier ;))

## Usage

You can use the tool easily. Simply `cd` into `Part 2 Submission`; the Gandhi corpus is already present in the `gandhi` directory. Now, run `pip install -r requirements.txt` to install `openai` **(Important: you need to install an older version of openai, namely 0.28, in order to use the ChatCompletion Endpoint)**





After this, the prompt to be given can be edited in line `79` of `tester.cpp`. Also, the API call can be edited at line `13` of `qna_tool.cpp`; this is what the various requests mean:



| Endpoint | Meaning |
|  --------  |  -------  |
| `api_call.py` | Standard GPT-3.5-turbo call using `openai` |
| `api_call_vertex.py` | Google PaLM Vertex call using `requests` (this uses the API set up earlier as a proxy, so we don't have to log in to Google Cloud when running this) |
| `api_call_fastchat.py` | Open-source HuggingFace model call using `openai` (We set this up on port 8001 earlier) |

Now, to run the inference and get output, just compile and run `tester.cpp`:
`g++ -o tester.cpp ./main && ./main`

Output will be printed to console and also outputted to `<filename>`

Congratulations, you did an AI thing! Now as for 4.5M$ in funding from your favorite investing fund!!
