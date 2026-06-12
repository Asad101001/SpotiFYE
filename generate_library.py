import os
import random

songs_dir = "assets/songs"
library_file = "assets/library.txt"

files = [f for f in os.listdir(songs_dir) if f.endswith('.mp3')]

lines = ["# title|artist|genre|duration_sec|playCount|rating|path|coverPath"]

for f in files:
    # Remove .mp3 extension
    name = f[:-4].strip()
    
    # Split by first dash if exists
    if " - " in name:
        artist, title = name.split(" - ", 1)
        artist = artist.strip()
        title = title.strip()
    else:
        artist = "Unknown"
        title = name
        
    genre = "Hip Hop"
    duration = random.randint(180, 260)
    playCount = random.randint(1000, 50000)
    rating = round(random.uniform(4.0, 5.0), 1)
    
    path = f"assets/songs/{f}"
    
    coverPath = ""
    if artist.startswith("Drake"):
        coverPath = "assets/covers/For All The Dogs.jpg"
    elif artist.startswith("Travis Scott"):
        coverPath = "assets/covers/Astroworld.jpg"
    elif artist.startswith("Metro Boomin"):
        coverPath = "assets/covers/HEROES & VILLAINS.jpg"
    else:
        coverPath = "assets/covers/For All The Dogs.jpg"
        
    lines.append(f"{title}|{artist}|{genre}|{duration}|{playCount}|{rating}|{path}|{coverPath}")

with open(library_file, "w", encoding="utf-8") as file:
    file.write("\n".join(lines))
    file.write("\n")

print(f"Generated {len(files)} tracks in {library_file}")
