#pragma once

#if (!defined(Debug) && !defined(Release))
#define Debug
#endif

#if (defined(Debug) && defined(Release))
#undef Release
#endif

#ifdef Debug
#define FACE_DATABASE_PATH "../data/face_database.csv"
#define SHAPE_PREDICTOR_PATH "../models/shape_predictor_68_face_landmarks.dat"
#define FACE_MODEL_PATH "../models/dlib_face_recognition_resnet_model_v1.dat"
#endif

#ifdef Release
#define FACE_DATABASE_PATH "./data/face_database.csv"
#define SHAPE_PREDICTOR_PATH "./models/shape_predictor_68_face_landmarks.dat"
#define FACE_MODEL_PATH "./models/dlib_face_recognition_resnet_model_v1.dat"
#endif
