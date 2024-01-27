#pragma once

typedef std::function<void()> Lambda;

typedef std::vector<double> Point;
typedef std::vector<Point> PointList;

typedef std::pair<int, Point> IndexPointPair;
typedef std::vector<IndexPointPair> IndexPointPairList;

typedef std::pair<int, double> ActivationRadiusPair;
typedef std::pair<Lambda, ActivationRadiusPair> LambdaActivationRadiusPair;
typedef std::pair<Point, LambdaActivationRadiusPair> LambdaWithPoint;
typedef std::vector<LambdaWithPoint> ListOfLambdaWithPoint;

extern LambdaActivationRadiusPair NoLambda;

// template <typename T>
// std::vector<T> slicing(std::vector<T> &v, int X, int Y);
bool PointContainedBetween(Point point1, Point point2, Point point);
IndexPointPairList GetIntersections(PointList path_points, Point circle_focus, double circle_radius);
double GetErrorFromPath(double x, double y, PointList path_points, int current_path_segment, int end_index);
double Cap(double input, double cap);