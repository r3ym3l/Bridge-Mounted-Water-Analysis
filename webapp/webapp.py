from dash import Dash, dcc, html
import dash_bootstrap_components as dbc
from dash.dependencies import Input, Output
import pandas as pd
import plotly.express as px
import requests

from datetime import datetime, timedelta

HEADER_MAP = {
    'Distance': 'Distance(m)',
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


def authenticate_notehub():
    auth_url = 'https://notehub.io/oauth2/token'
    auth_data = {
        'grant_type': 'client_credentials',
        'client_id': '92f93b92-4b8e-4587-abbb-3f16bd95373b',
        'client_secret': 'eb8c680499477f63dc25adbe3b0bdfa7bfebc5accc50d72da49153185dfa5277'
    }

    auth_response = requests.post(auth_url, data=auth_data)

    return auth_response


def extract_json_from_notehub():
    event_auth = auth_token.json()['token_type'] + ' ' + auth_token.json()['access_token']
    event_headers = {'Content-Type': 'application/x-www-form-urlencoded', 'Authorization': event_auth}
    event_url = 'https://api.notefile.net/v1/projects/app:2cd7bc69-cf7d-4599-9940-247d9fcf2f6c/events'

    event_response = requests.get(event_url, headers=event_headers)

    return event_response.json()



# --- Start of program ---

auth_token = authenticate_notehub()
df = update_data()
df['Distance(m)'] = df['Distance(mm)'] / 1000.0
df['Time'] = datetime.now() - timedelta(33) + (df.index * timedelta(hours=1))

app = Dash(__name__, external_stylesheets=[dbc.themes.MORPH])

navbar = dbc.NavbarSimple(
    children=[
        dbc.NavItem(dbc.NavLink('Source Code', href='https://github.com/r3ym3l/Bridge-Mounted-Water-Analysis')),
    ],
    brand='Bridge-mounted Water Analysis Module Data',
    brand_href='#',
    color='primary',
    dark=True,
    style={'margin-bottom': '2rem'}
)

app.layout = dbc.Container(
    [
        navbar,
        dcc.Dropdown(id='data-dropdown',
                     options=list(HEADER_MAP.keys()), value='Distance'),
        html.Div(id='main-content', children=[
            dcc.Graph(id='main-plot', style={'height': '80vh'})
        ]),
        dcc.Interval(
            id='interval-component',
            interval=3600e3,
            n_intervals=0
        )
    ],
    fluid=True
)

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
        figure = px.line(spectral_df, x=spectral_df.index, y=spectral_df.columns,
                         title='Water Turbidity')
    else:
        figure = px.line(
           df, x='Time', y=HEADER_MAP[option],
           title=option + ' Data'
        )

    figure.update_layout(font={
        'size': 16
    })
    return figure

app.run_server(debug=True)