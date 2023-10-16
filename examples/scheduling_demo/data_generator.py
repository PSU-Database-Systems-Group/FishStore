import random

names = [
    "John", "Mark", "Carl", "Steven", "Abbey", "Abbi", "Abbie", "Abbot", "Abbotsen", "Abbotson", "Abbotsun",
    "Abbott", "Abbottson", "Abby", "Abbye", "Jutta", "Juxon", "Jyoti", "Kablesh", "Kacerek", "Kacey", "Kachine",
    "Kacie", "Kacy", "Kaczer", "Kaden", "Kadner"
]

jobs = [
    "accountant", "actor", "actuary", "adhesive bonding machine tender", "adjudicator", "administrative assistant",
    "administrative services manager", "adult education teacher", "advertising manager", "advertising sales agent",
    "aerobics instructor", "aerospace engineer", "aerospace engineering technician", "agent",
    "agricultural engineer", "agricultural equipment operator", "agricultural grader", "agricultural inspector",
    "agricultural manager", "agricultural sciences teacher", "agricultural sorter", "agricultural technician",
    "agricultural worker", "air conditioning installer", "air conditioning mechanic", "air traffic controller",
    "aircraft cargo handling supervisor", "aircraft mechanic", "aircraft service technician", "airline copilot",
    "airline pilot"
]

people = []

for i in range(0, 5000):
    name = random.choice(names)
    job = random.choice(jobs)
    age = random.randint(20, 80)
    people.append((i, name, job, age))

with open("people.json", "w") as f:
    for p in people:
        f.write('{"name":"%s", "id":%d, "age":%d}\n' % (p[1], p[0], p[3]))

with open("jobs.json", "w") as f:
    for p in people:
        f.write('{"id":%d, "job":"%s"}\n' % (p[0], p[2]))
