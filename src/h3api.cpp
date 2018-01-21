#include <Rcpp.h>
#include <h3/h3api.h>

using namespace Rcpp;

double normalize(double degrees) {
  return degsToRads(degrees);
}

double denormalize(double radians) {
  double degrees = radsToDegs(radians);
  if (degrees > 180) {
    return -360 + degrees;
  }
  return degrees;
}

List GeoCoordToList(GeoCoord x) {
  return List::create(
    _["lat"] = denormalize(x.lat),
    _["lon"] = denormalize(x.lon)
  );
}

List GeoBoundaryToList(GeoBoundary x) {
  List result(x.numVerts);
  for(int i = 0; i < x.numVerts; ++i) {
    result[i] = GeoCoordToList(x.verts[i]);
  }
  return result;
}

GeoCoord ListToGeoCoord(List x) {
  if(x.containsElementNamed("lat") && x.containsElementNamed("lon")) {
    GeoCoord y;
    y.lat = normalize(x["lat"]);
    y.lon = normalize(x["lon"]);
  }
  stop("Input must be a list containing `lat` and `lon` members");
}

uint64_t getCIndexFromCoords(double lat, double lon, int res) {
  GeoCoord input;
  input.lat = normalize(lat);
  input.lon = normalize(lon);
  H3Index index = H3_EXPORT(geoToH3)(&input, res);
  if (index == 0) {
    stop("Error finding index. Check that your resolution is between 1-15.");
  }
  return index;
}

//' getIndexFromCoords returns the h3 index from a lat, lon, and resolution
//' @param lat latitude in degrees
//' @param lon longitude in degrees
//' @param resolution resolution, higher is more granular
// [[Rcpp::export]]
String getIndexFromCoords(double lat, double lon, int resolution) {
  uint64_t index = getCIndexFromCoords(lat, lon, resolution);
  char buf [32];
  h3ToString(index, buf, 32);
  return buf;
}

//' getBoundingHexFromCoords returns the h3 bounding hex from a lat, lon and resolution
//' @param lat latitude in degrees
//' @param lon longitude in degrees
//' @param resolution resolution, higher is more granular
// [[Rcpp::export]]
List getBoundingHexFromCoords(double lat, double lon, int resolution) {
  uint64_t h3 = getCIndexFromCoords(lat, lon, resolution);
  GeoBoundary y;
  H3_EXPORT(h3ToGeoBoundary)(h3, &y);
  return GeoBoundaryToList(y);
}

List getBoundingHexFromIndex(uint64_t index) {
  GeoBoundary y;
  H3_EXPORT(h3ToGeoBoundary)(index, &y);
  return GeoBoundaryToList(y);
}

//' getBoundingHexFromIndex takes an h3 index and returns the corresponding bounding hex
//' @param index h3 index as a hex representation character vector. See \code{\link{getIndexFromCoords}}
// [[Rcpp::export]]
List getBoundingHexFromIndex(String index) {
  uint64_t input = stringToH3(index.get_cstring());
  if (input == 0) {
    stop("Cannot parse an index from that string.");
  }
  return getBoundingHexFromIndex(input);
}

