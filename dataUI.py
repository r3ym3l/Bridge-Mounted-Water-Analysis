import pandas as pd
import csv
import matplotlib.pyplot as mp
# getting data from notehub
import datetime
import json
import requests

def spectral_ui():
    df = pd.read_csv('DATALOG.csv')
    list_of_column_names = list(df.columns)
    data = pd.DataFrame({'Wavelength (nm)':list_of_column_names[2:11]})
    data.to_csv('spectral_data.csv', index = False)
 
    # Open file
    with open('DATALOG.csv') as file_obj:

        # Skips the heading
        # Using next() method
        heading = next(file_obj)

        # Create reader object by passing the file 
        # object to reader method
        reader_obj = csv.reader(file_obj)

        # Iterate over each row in the csv file 
        # using reader object
        for row in reader_obj:
            col_name = row[-1]
            data[col_name] = row[2:11]
            data.to_csv('spectral_data.csv') 

        df = pd.read_csv('spectral_data.csv')
        list_of_column_names = list(df.columns)

        # plot multiple columns such as population and year from dataframe
        df.plot(x=list_of_column_names[1], y=list_of_column_names[2:], kind="line", figsize=(20, 10))

        # display plot
        mp.title("Turbid Water Data")
        mp.ylabel("Intensity Levels")
        mp.grid(color='gray', linestyle='dashed')
        mp.show()

def distance_ui():
    df = pd.read_csv('DATALOG.csv')
    list_of_column_names = list(df.columns)
 
    # plot multiple columns such as population and year from dataframe
    df.plot(x=list_of_column_names[0], y=list_of_column_names[1], kind="scatter", color="DarkBlue", figsize=(20, 10))
    
    # display plot
    mp.title("Ultrasonic Distance Data")
    mp.ylabel("Height (mm)")
    mp.show()

def extract_json_from_notehub():
    auth_url = "https://notehub.io/oauth2/token"
    auth_data = {
        "grant_type": "client_credentials",
        "client_id": "92f93b92-4b8e-4587-abbb-3f16bd95373b",
        "client_secret": "eb8c680499477f63dc25adbe3b0bdfa7bfebc5accc50d72da49153185dfa5277"
    }

    auth_response = requests.post(auth_url, data=auth_data)

    event_url = "https://api.notefile.net/v1/projects/app:2cd7bc69-cf7d-4599-9940-247d9fcf2f6c/events"
    event_auth = auth_response.json()["token_type"] + " " + auth_response.json()["access_token"]
    event_headers = {"Content-Type": "application/x-www-form-urlencoded", "Authorization": event_auth}
    
    event_response = requests.get(event_url, headers=event_headers)

    return event_response.json()

def process_json_to_csv(data_json: dict):
    curr_date = datetime.datetime.now().isoformat()
    # filter events to only the data we need
    data_list = []
    for i in data_json["events"]:
        if i["file"] == "sensors.qo":
            # will require when filtering out which data to include
            # log_time = i["body"]["Timestamp"] 
            # if log_time[0:10] == curr_date[0:10]:
            data_list.append(i)

    filtered_data_json = {"events":data_list}

    events = filtered_data_json["events"]
    
    # open a file for writing csv data into
    # data_file = open("datalog_sample.csv", "w", newline='')
    data_file = open("DATALOG.csv", "w", newline='')
    csv_writer = csv.writer(data_file)

    # Counter variable used for writing
    # headers to the CSV file
    count = 0
    
    for event in events:
        if count == 0:
    
            # Writing headers of CSV file
            header = event["body"].keys()
            csv_writer.writerow(header)
            count += 1
    
        # Writing data of CSV file
        csv_writer.writerow(event["body"].values())
    
    data_file.close()

# data_json = extract_json_from_notehub()
# process_json_to_csv(data_json)

spectral_ui()
distance_ui()
