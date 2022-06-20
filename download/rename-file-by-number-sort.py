#conding=utf8  
import os 

number=0
for path,dir_list,file_list in os.walk("E:\\AI\\opencv\\yolov5-6.1\\people_data\\images"):  
    for file_name in file_list:
        number = number + 1
        # print(str(number).zfill(6))
        file_path = os.path.join(path, file_name);  
        print(file_path)
        if number > 33:
            break
        # os.rename(file_path, os.path.join(path, str(number).zfill(6)) + ".jpg")
