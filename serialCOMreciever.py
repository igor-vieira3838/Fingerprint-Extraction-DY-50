import serial

def read_fingerprint_template(serial_port='COM6', baudrate=115000, timeout=1000):
    try:
        ser = serial.Serial(serial_port, baudrate, timeout=timeout)
        print(f"Conectado à {serial_port}")

        template = bytearray()
        
        while True:
            byte = ser.read(1)
            if byte:
                template.append(ord(byte))
                if len(template) == 512:
                    break

        ser.close()
        
        with open('fingerprint_template.bin', 'wb') as f:
            f.write(template)
        
        print(f"Template de impressão digital salvo em 'fingerprint_template.bin'")

    except serial.SerialException as e:
        print(f"Erro de comunicação: {e}")

if __name__ == "__main__":
    read_fingerprint_template()
