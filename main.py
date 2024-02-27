import asyncio
import os

import discord
from discord.ext import commands
import pyrebase


# variables that need to be private.
bot_token = os.environ.get('BOT_TOKEN')
api_key = os.environ.get('API_KEY')
guild_id = int(os.environ.get('GUILD_ID'))  # to change it to integer

firebaseConfig = {
    "apiKey": api_key,
    "authDomain": "disconnector-1d6c7.firebaseapp.com",
    "databaseURL": "https://disconnector-1d6c7-default-rtdb.europe-west1.firebasedatabase.app/",
    "projectId": "disconnector-1d6c7",
    "storageBucket": "disconnector-1d6c7.appspot.com",
    "messagingSenderId": "117441578438",
    "appId": "1:117441578438:web:c0b56e19542d61e406dec7",
    "measurementId": "G-5P4T2EGJ4W"
}

#initializing the firebase:
firebase = pyrebase.initialize_app(firebaseConfig)
db = firebase.database()




# bot instance
bot = commands.Bot(command_prefix="!")

# user  id to be kicked (nego)
target_user_id = 430405186528346123  # TODO : change this later to make it more flexible)

# func to kick the user from a voice channel
async def kick_user():
    """kicks the target_user_id user from their current voice channel"""
    guild = bot.get_guild(guild_id)  # Replace with the actual guild ID
    for member in guild.members:
        if member.id == target_user_id and member.voice:
            try:
                await member.move_to(None)
                print(f"User with ID {target_user_id} has been kicked from voice channel.")
            except discord.HTTPException as e:
                print(f"An error occurred while kicking the user: {e}")
            break

# monitor Firebase for changes
@bot.event
async def on_ready():
    """print a message when the bot is ready"""
    print(f"Logged in as {bot.user} (ID: {bot.user.id})")

    # set up the listener for changes in the Firebase database
    db.child("kick_trigger").on("value", lambda snapshot: asyncio.run(kick_user() if snapshot.val() else None))

# Run the bot
bot.run(bot_token)
