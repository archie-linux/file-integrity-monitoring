### Install Dependencies

- eval "$(/opt/homebrew/bin/brew shellenv)" # switch to arm64 homebrew
- brew install nlohmann-json
- brew install openssl

### Running the Program

- ./create_multiple_dirs # creates dummy directories and files with dummy data
- make file_integrity_monitoring
- ./file_integrity_monitoring

### Sample Output

<pre>
MacBook-Air-2:file-integrity-monitoring anish$ ./file_integrity_monitoring 
[DEBUG] File Path: ./dummy_dir1/file_7.txt
[DEBUG] File Path: ./dummy_dir1/file_5.txt
[DEBUG] File Path: ./dummy_dir1/file_4.txt
[DEBUG] File Path: ./dummy_dir1/file_6.txt
[INFO] New file detected, adding to database: ./dummy_dir1/file_4.txt
[DEBUG] File Path: ./dummy_dir1/file_1.txt
[INFO] New file detected, adding to database: ./dummy_dir1/file_5.txt
[INFO] New file detected, adding to database: ./dummy_dir1/file_7.txt
[DEBUG] File Path: ./dummy_dir1/file_2.txt
[DEBUG] File Path: ./dummy_dir1/file_3.txt
[INFO] New file detected, adding to database: ./dummy_dir1/file_6.txt
[INFO] New file detected, adding to database: ./dummy_dir1/file_2.txt
[DEBUG] File Path: ./dummy_dir1/file_10.txt
[INFO] New file detected, adding to database: ./dummy_dir1/file_1.txt[DEBUG] File Path: ./dummy_dir1/file_8.txt

[INFO] New file detected, adding to database: ./dummy_dir1/file_10.txt
[INFO] New file detected, adding to database: ./dummy_dir1/file_3.txt
[DEBUG] File Path: ./dummy_dir2/file_7.txt
[DEBUG] File Path: ./dummy_dir1/file_9.txt
[DEBUG] File Path: ./dummy_dir2/file_6.txt
[INFO] New file detected, adding to database: ./dummy_dir1/file_9.txt[INFO] New file detected, adding to database: ./dummy_dir2/file_6.txt
[DEBUG] File Path: ./dummy_dir2/file_4.txt[INFO] New file detected, adding to database: ./dummy_dir2/file_7.txt

[DEBUG] File Path: ./dummy_dir2/file_5.txt


### Modify some files

truncate -s 0 [file_name]

[DEBUG] File Path: ./dummy_dir2/file_10.txt
[DEBUG] File Path: ./dummy_dir2/file_8.txt
[DEBUG] File Path: ./dummy_dir2/file_9.txt
2025-2-9 12:32:32 - ALERT: Unauthorized modification detected in ./dummy_dir2/file_5.txt
[DEBUG] File Path: ./dummy_dir1/file_7.txt
[DEBUG] File Path: [DEBUG] File Path: ./dummy_dir1/file_4.txt./dummy_dir1/file_6.txt

[DEBUG] File Path: ./dummy_dir1/file_1.txt
[DEBUG] File Path: [DEBUG] File Path: ./dummy_dir1/file_3.txt
./dummy_dir1/file_2.txt
[DEBUG] File Path: ./dummy_dir1/file_10.txt
[DEBUG] File Path: ./dummy_dir1/file_8.txt
[DEBUG] File Path: ./dummy_dir1/file_9.txt

[DEBUG] File Path: ./dummy_dir2/file_5.txt
[DEBUG] File Path: ./dummy_dir2/file_2.txt
[DEBUG] File Path: ./dummy_dir2/file_6.txt
[DEBUG] File Path: ./dummy_dir2/file_3.txt
[DEBUG] File Path: ./dummy_dir2/file_10.txt
[DEBUG] File Path: ./dummy_dir2/file_8.txt
[DEBUG] File Path: ./dummy_dir2/file_1.txt[DEBUG] File Path: 
[WARNING] File modified: ./dummy_dir2/file_10.txt
./dummy_dir2/file_9.txt
2025-2-9 12:32:42 - ALERT: Unauthorized modification detected in ./dummy_dir2/file_10.txt

</pre>
