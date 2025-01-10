import matplotlib.pyplot as plt
import numpy as np
import re
import sys

# Function to read data from log file
def read_log_file(file_path):
    # Initialize lists for storing extracted values
    timestamps = []
    cpu_times = []
    lpm_times = []
    deep_lpm_times = []
    tx_times = []
    rx_times = []
    energy_tx = []
    energy_rx = []
    energy_cpu = []
    energy_lpm = []
    energy_deep_lpm = []

    # Define a regex pattern to match log entries
    log_pattern = re.compile(
        r"\[Timestamp: (\d+)\] CPU time: (\d+) ms, LPM time: (\d+) ms, "
        r"Deep LPM time: (\d+) ms, Tx time: (\d+) ms, Rx time: (\d+) ms, "
        r"Energy \(TX\): ([\d\.]+) J, Energy \(RX\): ([\d\.]+) J, "
        r"Energy \(CPU\): ([\d\.]+) J, Energy \(LPM\): ([\d\.]+) J, "
        r"Energy \(Deep LPM\): ([\d\.]+) J"
    )

    # Read the file and parse each line
    with open(file_path, 'r') as file:
        for line in file:
            match = log_pattern.match(line)
            if match:
                # Extract matched groups and convert to appropriate types
                timestamps.append(float(match.group(1)))
                cpu_times.append(float(match.group(2)))
                lpm_times.append(float(match.group(3)))
                deep_lpm_times.append(float(match.group(4)))
                tx_times.append(float(match.group(5)))
                rx_times.append(float(match.group(6)))
                energy_tx.append(float(match.group(7)))
                energy_rx.append(float(match.group(8)))
                energy_cpu.append(float(match.group(9)))
                energy_lpm.append(float(match.group(10)))
                energy_deep_lpm.append(float(match.group(11)))

    return timestamps, cpu_times, lpm_times, deep_lpm_times, tx_times, rx_times, energy_tx, energy_rx, energy_cpu, energy_lpm, energy_deep_lpm

# Main function
def main(kwargs):
    # Read data from log.txt
    file_path = kwargs[1]
    timestamps, cpu_times, lpm_times, deep_lpm_times, tx_times, rx_times, energy_tx, energy_rx, energy_cpu, energy_lpm, energy_deep_lpm = read_log_file(file_path)

    # Get the last elements from each list
    last_timestamp = timestamps[-1]
    last_cpu_time = cpu_times[-1]
    last_tx_time = tx_times[-1]
    last_rx_time = rx_times[-1]
    last_energy_tx = energy_tx[-1]
    last_energy_rx = energy_rx[-1]
    last_energy_cpu = energy_cpu[-1]

    # Print the last elements (optional for debugging)
    print(f"Last Timestamp: {last_timestamp}")
    print(f"Last CPU Time (ms): {last_cpu_time}")
    print(f"Last TX Time (ms): {last_tx_time}")
    print(f"Last RX Time (ms): {last_rx_time}")
    print(f"Last Energy (TX) (J): {last_energy_tx}")
    print(f"Last Energy (RX) (J): {last_energy_rx}")
    print(f"Last Energy (CPU) (J): {last_energy_cpu}")

    # Plot the last values in 3-column format (TX, RX, CPU)
    plt.figure(figsize=(14, 6))

    labels = ['TX Energy', 'RX Energy', 'CPU Energy']
    values = [last_energy_tx, last_energy_rx, last_energy_cpu]

    # Plot as a bar chart
    plt.bar(labels, values, color=['blue', 'green', 'orange'], alpha=0.7)

    plt.ylabel('Energy (J)')
    plt.title(f'Last Energy Consumption for TX, RX, and CPU at Timestamp {last_timestamp}')
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    plt.show()

    # Linear Plot for Times (TX, RX, CPU)
    plt.figure(figsize=(14, 6))

    plt.bar(['TX Time', 'RX Time', 'CPU Time'], [last_tx_time, last_rx_time, last_cpu_time],
            color=['blue', 'green', 'red'], alpha=0.7)

    plt.ylabel('Time (ms)')
    plt.title(f'Last TX, RX, and CPU Time at Timestamp {last_timestamp}')
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    plt.show()

# Calling main function
if __name__ == "__main__":
    main(sys.argv)
