import os
import subprocess
import pandas as pd
from datetime import datetime


def main():
    site_list = [
        'www.ufrj.br',
        'www.ucla.edu',
        'www.phil.cam.ac.uk',
        'www.adelaide.edu.au'
    ]
    for host in site_list:
        for _ in range(10):
            output = traceroute('*******', host)
            save_to_csv('/Users/eduardovillani/git/redes-tcp-ip-tp1/output.csv', output)


def save_to_csv(filename, data):
    print(f'Saving Data to CSV, file {filename}')
    df = pd.DataFrame(data)
    # csv_data = df.to_csv(index=False)
    df.to_csv(filename, index=False, mode='a+', header=False)


def traceroute(sudo_password, host):
    sudoPassword = sudo_password
    command = f'tcptraceroute {host}'.split()

    print('Sending Command')
    cmd1 = subprocess.Popen(['echo', sudoPassword], stdout=subprocess.PIPE)
    cmd2 = subprocess.Popen(['sudo', '-S'] + command, stdin=cmd1.stdout, stdout=subprocess.PIPE)
    output = cmd2.stdout.read()
    output = str(output).replace('b\'', '').replace('\'', '').replace('[open]', '')[:-1].split('\\n')
    print('Cleaning data')
    new_list = []
    for index, o in enumerate(output):
        if '* * *' in o:
            continue
        output[index] = o.replace('\\', "").replace('   ', '  ').replace('*','  ').split('  ')
        
        output[index].append(datetime.now().strftime("%Y/%m/%dT%H:%M:%S"))
        output[index].append(host)
        print(output[index])
        new_list.append(output[index])
    return new_list


if __name__ == '__main__':
    main()
