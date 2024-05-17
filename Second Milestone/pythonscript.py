import pandas as pd
import plotly.express as px


data = pd.read_csv('TESTFINAL1.csv', names=['lat','lon'])  # if your csv doesn't have a header

data.info()

fig = px.scatter_mapbox(data, lat='lat', lon='lon', hover_data=['lat','lon'])
fig.update_layout(mapbox_style='open-street-map')
fig.update_layout(margin={"r":0,"t":0,"l":0,"b":0})
fig.show()
