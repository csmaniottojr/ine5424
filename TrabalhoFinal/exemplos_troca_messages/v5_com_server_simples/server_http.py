from http.server import BaseHTTPRequestHandler, HTTPServer
import serial

DEVICE='/dev/ttyACM0'
ser = serial.Serial(
    port=DEVICE,		
    baudrate=115200,
    timeout=1.0,	
    write_timeout=1.0)
 
# HTTPRequestHandler class
class testHTTPServer_RequestHandler(BaseHTTPRequestHandler):
 
    # GET
    def do_GET(self):
        # Send response status code
        self.send_response(200)
        
        print('Path: %s' % self.path)
        if self.path.endswith(".html") or self.path == '/':
            # Send headers
            self.send_header('Content-type','text/html')
            self.end_headers()
     
            if not ser.is_open:
                print('Abrindo...')
                ser.open()
            n = ser.write(b':A106AB\r\n')
            # Send message back to client
            message = "Enviados: %d bytes!" % n
            # Write content as utf-8 data
            self.wfile.write(bytes(message, "utf8"))
            ser.close()
            print('Fechando...')
        return
 
def run():
    print('starting server...')
 
    # Server settings
    # Choose port 8080, for port 80, which is normally used for a http server, you need root access
    server_address = ('127.0.0.1', 8081)
    httpd = HTTPServer(server_address, testHTTPServer_RequestHandler)
    print('running server...')
    httpd.serve_forever()

run()
