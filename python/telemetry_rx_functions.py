import serial

HEADER = 'BEG'
SUFFIX = 'END\n'
MSG_LENGTH = 100  #Size of uint8 buffer downlinked

PACKET_LENGTH = len(HEADER) + MSG_LENGTH + len(SUFFIX)

# Returns true if the provided binary string starts with HEADER.
def contains_header(binary_string):
    if len(binary_string) >= 3:
        try:
            string_header = binary_string[0:3].decode("ascii")
        except UnicodeDecodeError:
            return False
        if string_header == HEADER:
            return True
    return False

# Returns true if the provided binary string is a complete packet.
def is_complete_packet(binary_string):
    is_correct_length = (len(binary_string) == PACKET_LENGTH)
    if is_correct_length and contains_header(binary_string):
        try:
            string_suffix = binary_string[-4:].decode("ascii")
        except UnicodeDecodeError:
            return False
        if string_suffix == SUFFIX:
            return True
    return False

# Reads incoming packets on serial_port, and appends complete packets
# to packet_list.
def log_received_packets(serial_port, packet_list):
    current_packet = b''
    while serial_port.is_open:
        received_string = serial_port.readline()

        if contains_header(received_string):
            current_packet = received_string

        elif len(current_packet) > 0:
            current_packet += received_string

        if is_complete_packet(current_packet):
            packet_contents = current_packet[3:-4]
            packet_list.append(packet_contents)

# Returns next complete packet received over serial port.
def get_next_packet(serial_port):
    current_packet = b''
    while not is_complete_packet(current_packet):
        received_string = serial_port.readline()

        if contains_header(received_string):
            if len(current_packet) > 0:
                print('Incomplete Packet Dropped')
            current_packet = received_string

        elif len(current_packet) > 0:
            current_packet += received_string

        if is_complete_packet(current_packet):
            packet_contents = current_packet[3:-4]
            return packet_contents

