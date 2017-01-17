import sys, UnixSockets

if len(sys.argv) != 2:
    print('Usage: python {0} <socket>'.format(sys.argv[0]))
    quit()

socketName = sys.argv[1]

try:
    mySocket = UnixSockets.Socket(socketName)
except Exception as e:
    print('Error opening socket {0}: "{1}"'.format(socketName, e))
    quit()

words = [ 'Hi there', 'Just testing you from python', 'Third message', 'Another message, just before leaving', 'So long']

for word in words:
    try:
        writeRes = mySocket.write(word)
        print('* Sent "{0}", {1} bytes sent'.format(word, writeRes))
        resp = mySocket.read()
        print('  Listener response: {0}'.format(resp))
    except IOError as e:
        print('Error communicating with listener: "{0}"'.format(e))
        break
