import datetime, serial, picamera, math
from geopy.distance import vincenty

camera = picamera.PiCamera()

# get latlong
def get_latlon():
    gps = serial.Serial('/dev/ttyACM0', 115200)
    latlong = gps.readline()
    lat = float(latlong.split(",")[0])
    lon = float(latlong.split(",")[1][:-2]) # remove '\r\n' at the end
    lon = lon/100 # accounts for bad Linkit One example code
    return lat, lon
lat, lon = get_latlon()
print "Found GPS location of {}, {}".format(lat, lon)

# check latlong accuracy
for x in range(2):
    lat2, lon2 = get_latlon()
    gps_readings_diff = vincenty((lat, lon), (lat2, lon2)).meters
        assert gps_readings_diff > 20, "GPS accuracy isn't good enough"

# set GPSLatitude (credit: http://bit.ly/1Rg78PV)
camera.exif_tags['GPS.GPSLatitudeRef'] = 'N' if lat > 0 else 'S'
alat = math.fabs(lat)
dlat = int(alat)
mlat = int(60 * (alat - dlat))
slat = int(6000 * (60 * (alat - dlat) - mlat))
camera.exif_tags['GPS.GPSLatitude'] = '%d/1,%d/1,%d/100' % (dlat, mlat, slat)

# set GPSLongitude (credit: http://bit.ly/1pnSIrl)
camera.exif_tags['GPS.GPSLongitudeRef'] = 'E' if lon > 0 else 'W'
alon = math.fabs(lon)
dlon = int(alon)
mlon = int(60 * (alon - dlon))
slon = int(6000 * (60 * (alon - dlon) - mlon))
camera.exif_tags['GPS.GPSLongitude'] = '%d/1,%d/1,%d/100' % (dlon, mlon, slon)

# take picture
timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M")
filename = timestamp + '.jpg'
camera.capture(filename)
print "Image saved as {}".format(filename)
