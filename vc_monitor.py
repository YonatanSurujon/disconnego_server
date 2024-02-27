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

# initialize the firebase
firebase = pyrebase.initialize_app(firebaseConfig)
db = firebase.database()


# Discord bot setup
intents = discord.Intents.default()
intents.members = True
bot = commands.Bot(command_prefix="!", intents=intents)

# IDs for specific users and channels
nigit_id = 430405186528346123
pair_id = 980401934613774367
channel_ids = {
    "vc1": 980452588245880833,
    "vc2": 980452646794166282,
    "vc3": 980452704390377532,
    "vc4": 980452768416407642,
    "vc5": 980452799202611200,
    "pair": 980401934613774367
}
# channel lists (they are empty at first)
channels = {
    "vc1": [],
    "vc2": [],
    "vc3": [],
    "vc4": [],
    "vc5": [],
    "pair": [],
}


@bot.event
async def on_voice_state_update(member, before, after):
    """
    Handles voice state updates for members.

    - Updates internal channel lists based on member movements.
    - Writes channel data to the Firebase database.
    """

    @bot.event
    async def on_voice_state_update(member, before, after):
        member_id = member.id

        # Handle leaving a voice channel
        if not after.channel:
            for channel_name in channels:
                if member_id in channels[channel_name]:
                    channels[channel_name].remove(member_id)
                    break  # Exit the loop after removing from one channel

        # Handle joining or moving between voice channels
        else:
            channel_id = after.channel.id
            channel_name = channel_ids.get(channel_id)  # Get channel name from ID

            if channel_name:
                if member_id == nigit_id:
                    channels[channel_name].insert(0, member_id)  # Prioritize nigit_id
                else:
                    channels[channel_name].append(member_id)

                # If leaving a previous channel, remove from its list
                if before.channel and before.channel.id in channel_ids.values():
                    previous_channel = channel_ids.get(before.channel.id)
                    channels[previous_channel].remove(member_id)

        # Update Firebase with channel data
        db.child("channels").set(channels)

bot.run(bot_token)
