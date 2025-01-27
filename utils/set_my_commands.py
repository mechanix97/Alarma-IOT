import requests

# Token de tu bot de Telegram
bot_token = "TOKEN"

# URL base de la API de Telegram
base_url = f"https://api.telegram.org/bot{bot_token}/"

# Comandos que quieres configurar
commands = [
    {"command": "/alarma_on", "description": "Enciende la alarma"},
    {"command": "/alarma_off", "description": "Apaga la alarma"}
]

# Función para configurar los comandos
def set_commands():
    url = base_url + "setMyCommands"
    payload = {
        "commands": commands
    }
    
    # Realizar la solicitud POST
    response = requests.post(url, json=payload)
    
    # Comprobar la respuesta
    if response.status_code == 200:
        print("Comandos configurados correctamente.")
    else:
        print(f"Error al configurar los comandos: {response.status_code}")
        print(response.text)

# Llamar a la función para setear los comandos
set_commands()
