import openai
client = openai.OpenAI(api_key="sk-b_kIeSAO3v5itDGKVH2wSQ", 
base_url="https://ai.blsc.cn")

response = client.chat.completions.create(
    model="GLM-4.6",  # model to send to the proxy
    messages=[
        {
            "role": "user",
            "content": "Hello world"        
        }
    ]
)
print(response)