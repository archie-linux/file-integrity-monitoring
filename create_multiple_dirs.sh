#!/bin/bash

# Directories to create
DIRS=("dummy_dir1" "dummy_dir2")

# Create directories and generate files
for DIR in "${DIRS[@]}"; do
    mkdir -p "$DIR"
    
    for i in {1..10}; do
        echo "This is file_$i in $DIR. Generated on $(date)" > "$DIR/file_$i.txt"
    done

    echo "Created 10 dummy files in '$DIR'"
done

echo "✅ Dummy data setup complete!"

