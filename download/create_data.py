import os
import numpy as np
import scipy.io as sio
import shutil
from lxml.etree import Element, SubElement, tostring
from xml.dom.minidom import parseString
import cv2

VOCRoot = 'E:/AI/opencv/yolov5-6.1/people_data'
widerDir = 'E:/AI/opencv/download/WiderPerson'  # 数据集所在的路径

def make_voc_dir():
    # labels 目录若不存在，创建labels目录。若存在，则清空目录
    if not os.path.exists(VOCRoot + '/Annotations'):
        os.makedirs(VOCRoot + '/Annotations')
    if not os.path.exists(VOCRoot + '/ImageSets'):
        os.makedirs(VOCRoot + '/ImageSets')
        os.makedirs(VOCRoot + '/ImageSets/Main')
    # if not os.path.exists(VOCRoot + '/JPEGImages'):
    #     os.makedirs(VOCRoot + '/JPEGImages')
    if not os.path.exists(VOCRoot + '/images'):
        os.makedirs(VOCRoot + '/images')


if __name__ == '__main__':
# < class_label =1: pedestrians > 行人
                # < class_label =2: riders >      骑车的
                # < class_label =3: partially-visible persons > 遮挡的部分行人
                # < class_label =4: ignore regions > 一些假人，比如图画上的人
                # < class_label =5: crowd > 拥挤人群，直接大框覆盖了

    # classes = {'1': 'pedestrians',
    #            '2': 'riders',
    #            '3': 'partially',
    #            '4':'ignore',
    #            '5':'crowd'
    #            }
    classes = {'1': 'people',
                '2': 'people',
                '3': 'people',
                '4':'people',
                '5':'people'
    }
    #这里如果自己只要人，可以把1-5全标记为people，也可以根据自己场景需要筛选

    #把下面的py文件运行3遍，第一遍运行train.txt文件，第二遍运行val.txt文件，并且注释掉make_voc_dir函数的调用，自己可以根据自己的场景需要进行数据集的筛选，第三遍运行test.txt文件，并注释掉with open(label_path) as file和with open(xml_path, ‘wb’) as f里面的内容


    # wider_path = widerDir + '/train.txt'
    # wider_path = widerDir + '/val.txt'
    wider_path = widerDir + '/test.txt'

    #这个函数第一次用注释掉，后面就要加注释了
    # make_voc_dir()

    with open(wider_path, 'r') as f:
        imgIds = [x for x in f.read().splitlines()]

    for imgId in imgIds:
        objCount = 0  # 一个标志位，用来判断该img是否包含我们需要的标注
        filename = imgId + '.jpg'
        img_path = widerDir + '/images/' + filename
        # print('Img :%s' % img_path)
        shutil.copy(img_path, VOCRoot + '/images/' + filename)
        # continue

        #转换voc为yolo格式
        label_path = img_path.replace('images', 'Annotations') + '.txt'
        if os.path.exists(label_path):
            img = cv2.imread(img_path)
            width = img.shape[1]  # 获取图片尺寸
            height = img.shape[0]  # 获取图片尺寸 360

            node_root = Element('annotation')
            node_folder = SubElement(node_root, 'folder')
            node_folder.text = 'images'
            node_filename = SubElement(node_root, 'filename')
            node_filename.text = 'VOC2007/images/%s' % filename
            node_size = SubElement(node_root, 'size')
            node_width = SubElement(node_size, 'width')
            node_width.text = '%s' % width
            node_height = SubElement(node_size, 'height')
            node_height.text = '%s' % height
            node_depth = SubElement(node_size, 'depth')
            node_depth.text = '3'


            with open(label_path) as file:
                line = file.readline()
                count = int(line.split('\n')[0])  # 里面行人个数
                line = file.readline()
                while line:
                    cls_id = line.split(' ')[0]
                    xmin = int(line.split(' ')[1]) + 1
                    ymin = int(line.split(' ')[2]) + 1
                    xmax = int(line.split(' ')[3]) + 1
                    ymax = int(line.split(' ')[4].split('\n')[0]) + 1
                    line = file.readline()

                    cls_name = classes[cls_id]

                    obj_width = xmax - xmin
                    obj_height = ymax - ymin

                    difficult = 0
                    if obj_height <= 6 or obj_width <= 6:
                        difficult = 1

                    node_object = SubElement(node_root, 'object')
                    node_name = SubElement(node_object, 'name')
                    node_name.text = cls_name
                    node_difficult = SubElement(node_object, 'difficult')
                    node_difficult.text = '%s' % difficult
                    node_bndbox = SubElement(node_object, 'bndbox')
                    node_xmin = SubElement(node_bndbox, 'xmin')
                    node_xmin.text = '%s' % xmin
                    node_ymin = SubElement(node_bndbox, 'ymin')
                    node_ymin.text = '%s' % ymin
                    node_xmax = SubElement(node_bndbox, 'xmax')
                    node_xmax.text = '%s' % xmax
                    node_ymax = SubElement(node_bndbox, 'ymax')
                    node_ymax.text = '%s' % ymax
                    node_name = SubElement(node_object, 'pose')
                    node_name.text = 'Unspecified'
                    node_name = SubElement(node_object, 'truncated')
                    node_name.text = '0'

            # image_path = VOCRoot + '/images/' + filename

            xml = tostring(node_root, pretty_print=True)  # 'annotation'
            dom = parseString(xml)
            xml_name = filename.replace('.jpg', '.xml')
            xml_path = VOCRoot + '/Annotations/' + xml_name
            with open(xml_path, 'wb') as f:
                f.write(xml)




