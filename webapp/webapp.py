from dash import Dash, dcc, html
from dash.dependencies import Input, Output
import pandas as pd
import plotly.express as px
import requests

HEADER_MAP = {
    'Distance': 'Distance(mm)',
    'Temperature': 'Temperature(Celsius)',
    'Spectral Data': 'F1(405-425)nm'
}

SPECTRAL_HEADERS = [
    'F1(405-425nm)',
    'F2(435-455nm)',
    'F3(470-490nm)',
    'F4(505-525nm)',
    'F5(545-565nm)',
    'F6(580-600nm)',
    'F7(620-640nm)',
    'F8(670-690nm)',
    'NIR(900nm)'
]

def update_data():
    events = extract_json_from_notehub()
    df = pd.DataFrame()
    for event_data in events['events']:
        event = event_data
        if event['file'] == 'sensors.qo':
            row_data = event['body']
            row = pd.json_normalize(row_data)
            df = pd.concat([df, row], axis='index', ignore_index=True)
    return df


def extract_json_from_notehub():
    auth_url = 'https://notehub.io/oauth2/token'
    auth_data = {
        'grant_type': 'client_credentials',
        'client_id': '92f93b92-4b8e-4587-abbb-3f16bd95373b',
        'client_secret': 'eb8c680499477f63dc25adbe3b0bdfa7bfebc5accc50d72da49153185dfa5277'
    }

    auth_response = requests.post(auth_url, data=auth_data)

    event_url = 'https://api.notefile.net/v1/projects/app:2cd7bc69-cf7d-4599-9940-247d9fcf2f6c/events'
    event_auth = auth_response.json()['token_type'] + ' ' + auth_response.json()['access_token']
    event_headers = {'Content-Type': 'application/x-www-form-urlencoded', 'Authorization': event_auth}
    
    event_response = requests.get(event_url, headers=event_headers)

    return event_response.json()

df = update_data()

app = Dash(__name__)
app.layout = html.Div([
    html.H1('Bridge-mounted Water Analysis Module Data'),
    dcc.Dropdown(id='data-dropdown',
                 options=list(HEADER_MAP.keys()), value='Distance'),
    html.Div(id='main-content', children=[
        dcc.Graph(id='main-plot', style={'height': '90vh'})
    ]),
    dcc.Interval(
        id='interval-component',
        interval=3600e3,
        n_intervals=0
    )
])


# @app.callback(Output('main-content', 'children'),
#               Input('interval-component', 'n_intervals'))
# def update_plots(n):
#     df = update_data()
#     return

@app.callback(Output('main-plot', 'figure'),
              Input('data-dropdown', 'value'))
def change_data(option):
    if option == 'Spectral Data':
        spectral_df = df[SPECTRAL_HEADERS]
        figure = px.line(spectral_df, x=spectral_df.index, y=spectral_df.columns)
    else:
        figure = {
            'data': [
                {
                    'x': df.index,
                    'y': df[HEADER_MAP[option]],
                    'type': 'lines'
                }
            ],
            'layout': {
                'title': HEADER_MAP[option]
            }
        }
    return figure

app.run_server(debug=True)