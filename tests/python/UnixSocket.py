import UnixSockets



class unixSocket:
    def __init__(self, socketName):
        try:
            self.fd = UnixSockets.open(socketName)
        except Exception as e:
            print('Error opening file: {0}'.format(list(e)[1]))

    def close(self):
        UnixSockets.close(self.fd)

    def write(self, s):
        return UnixSockets.write(self.fd, s)

    def read(self):
        return UnixSockets.read(self.fd)

words = [ 'First', 'Second', 'Onther one', 'bytes the dust' ]

mySocket = unixSocket('/tmp/mysock')

for word in words:
    write_res = mySocket.write(word)
    print('* Sent "{0}", {1} bytes sent ack'.format(word, write_res))
    resp = mySocket.read()
    print('  Listener response: {0}'.format(resp))

mySocket.close()
