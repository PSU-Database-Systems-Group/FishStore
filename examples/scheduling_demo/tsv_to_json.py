# Written by Microsoft Copilot in Edge, so may have bugs

import csv
import json


def tsv_to_json(tsv_file, json_file):
    with open(tsv_file, newline='') as f:
        reader = csv.reader(f, delimiter='\t')
        headers = next(reader)
        data = [dict(zip(headers, row)) for row in reader]
    with open(json_file, 'w') as f:
        for record in data:
            json.dump(record, f)
            f.write('\n')
        # json.dump(data, f, indent=4)

tsv_to_json("/scratch/mnorfolk/Data/events_set1/srilanka_floods_2017/srilanka_floods_2017_test.tsv","/scratch/mnorfolk/Data/json/srilanka_floods_2017.json")