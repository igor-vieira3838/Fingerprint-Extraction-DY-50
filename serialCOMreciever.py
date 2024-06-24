import serial

# Adjust the serial port name and baud rate as needed
ser = serial.Serial('COM3', 9600)
template_data = b''

print("Waiting for template data...")

while True:
    if ser.in_waiting > 0:
        data = ser.read(ser.in_waiting)
        template_data += data
        if data.endswith(b'\n'):  # End of template data
            break

# Save the template data to a file
with open('fingerprint_template.dat', 'wb') as file:
    file.write(template_data)

print("Template data saved to fingerprint_template.dat")
