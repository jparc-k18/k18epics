#!/usr/bin/env python3

import os
import slack

#client = slack.WebClient(token=os.environ['SLACK_API_TOKEN_E42'])
e42_token = 'xoxb-532855880339-1811675186800-JHIgoMwabhIyu6gWS44ZiOaZ'
client = slack.WebClient(token=e42_token)

channel = '#alarm'
k18_url = 'http://k18server.intra.j-parc.jp/k18epics/epics/'
tpc_url = 'http://www-online.kek.jp/~sks/e42/hyptpc/'

#______________________________________________________________________________
def send(message):
  ''' send slack message '''
  message = f'Received EPICS alarm : {message}'
  web_message = f'Check {k18_url} and {tpc_url}'
  response = client.chat_postMessage(
    channel=channel,
    text=message,
    attachments=[{
      'color': '#000000',
      'fallback': message,
      'text': web_message,
    }]
  )
  return response

#______________________________________________________________________________
if __name__ == '__main__':
  pass
#send('(test)')
