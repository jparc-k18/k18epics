#!/usr/bin/env python3

import slack
import os
import time

keep = 60*60*24*7

e42_token = 'xoxb-532855880339-1811675186800-JHIgoMwabhIyu6gWS44ZiOaZ'
client = slack.WebClient(token=e42_token)

channel = '#alarm'
channel_id = 'CGV3FQDD5' # alarm

#______________________________________________________________________________
if __name__ == '__main__':
  while True:
    try:
      response = client.conversations_history(channel=channel_id)
      # print(response)
      for m in response['messages']:
        if ('bot_profile' in m and m['bot_profile']['name'] == 'Epics Alarm'
            and time.time() - float(m['ts']) > keep):
          print(m)
          res = client.chat_delete(channel=channel_id, ts=m['ts'])
          print(f'Deleted  responce = {res}')
    except slack.errors.SlackApiError as err:
      print(err)
    print('sleeping...')
    time.sleep(3600)
