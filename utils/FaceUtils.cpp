#include "FaceUtils.h"

FaceUtils faceUtils;

FaceUtils::FaceUtils() {
    detector = get_frontal_face_detector();
    deserialize(SHAPE_PREDICTOR_PATH) >> predictor;
    deserialize(FACE_MODEL_PATH) >> net;
}

int FaceUtils::detectFace(matrix<dlib::rgb_pixel>& mat, full_object_detection& face, bool checkBound) {
    std::vector<dlib::rectangle> rects;

    rects = detector(mat);

    if (rects.size() == 1)
    {
        face = predictor(mat, rects.front());
        if (face.num_parts() > 0) {
            if (checkBound) {
                if (checkRectInBound(rects.front())) {
                    return 0;
                }
                else {
                    return 3;
                }
            }
            else {
                return 0;
            }
        }
        else {
            return 1;
        }
    }
    else if (rects.empty()) {
        return 1;
    }
    else {
        return 2;
    }
}

void FaceUtils::saveFaceData(const std::string& name, matrix<float, 0, 1>& des) {
    ofstream ofs;
    ofs.open(FACE_DATABASE_PATH, ios::out | ios::app);
    ofs << name << ',';
    for (int i = 0; i < 128; ++i) {
        if (i != 127) ofs << to_string(des(i)) << ',';
        else ofs << to_string(des(i));
    }
    ofs << endl;
    ofs.close();
}

bool FaceUtils::checkRectInBound(const dlib::rectangle &rect) {
//    qDebug("top: %ld, left: %ld, bottom: %ld, right: %ld", rect.top(), rect.left(), rect.bottom(), rect.right());
    long p1 = (rect.top() - 200) * (rect.top() - 200) + (rect.left() - 200) * (rect.left() - 200);
    long p2 = (rect.bottom() - 200) * (rect.bottom() - 200) + (rect.right() - 200) * (rect.right() - 200);
    return p1 <= 200 * 200 && p2 <= 200 * 200;
}

void FaceUtils::getFaceDescriptor(matrix<dlib::rgb_pixel> &mat, full_object_detection &face, matrix<float, 0, 1> &des) {
    matrix<dlib::rgb_pixel> face_chip;
    extract_image_chip(mat, get_face_chip_details(face, 150, 0.25), face_chip);
    std::vector< matrix<dlib::rgb_pixel> > faces;
    faces.push_back(std::move(face_chip));
    std::vector< matrix<float, 0, 1> > descriptors = net(faces);
    des = descriptors.front();
}

int FaceUtils::recogniseFace(matrix<float, 0, 1> &tar, string& name) {
    pair < matrix<float, 0, 1>, float > can({}, FLT_MAX);
    ifstream ifs;
    string line;
    int ret = -1;
    int i = 0;

    ifs.open(FACE_DATABASE_PATH, ios::in);
    getline(ifs, line);
    while (getline(ifs, line)) {
        matrix<float, 128, 1> des;
        istringstream iss;
        string item;
        string tmpName;
        bool first = true;
        int j = 0;
        
        iss.str(line);
        while (getline(iss, item, ',')) {
            if (first) {
                tmpName = item;
                first = false;
            }
            else {
                des(j) = strtof(item.c_str(), nullptr);
                ++j;
            }
        }

//        cout << "des: " << trans(des) << endl;
        float d = dlib::length(tar - des);
//        cout << "distance: " << d << endl;
        if (d < DEFAULT_THRESHOLD && d < can.second) {
            name = tmpName;
            can.first = des;
            can.second = d;
            ret = i;
        }

        ++i;
    }
    ifs.close();

    return ret;
}
