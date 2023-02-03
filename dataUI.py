import pandas as pd
import csv
import matplotlib.pyplot as mp

df = pd.read_csv('DATALOG.csv')
list_of_column_names = list(df.columns)
data = pd.DataFrame({'Wavelength (nm)':list_of_column_names[3:12]})
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
        col_name = row[0]
        data[col_name] = row[3:12]
        data.to_csv('spectral_data.csv') 
        
df = pd.read_csv('spectral_data.csv')
list_of_column_names = list(df.columns)
 
# plot multiple columns such as population and year from dataframe
df.plot(x=list_of_column_names[1], y=list_of_column_names[2:],
        kind="line", figsize=(20, 10))
 
# display plot
mp.title("Turbid Water Data")
mp.ylabel("Intensity Levels")
mp.show()