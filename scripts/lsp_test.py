#py -3

import sys
import socket


def main():
    connection = socket.create_connection(("locahost", 2087))
    print("Connection OK ?", connection is not None)


if __name__ == "__main__":
    main()
