
#' getHex gets the h3 hexagon at the index corresponding to the given coordinates and the given resolution
#' @param lat latitude in degrees
#' @param lon longitude in degrees
#' @param resolution resolution, higher is more granular
getHex <- function(lat = 29.7632836, lon = -95.3632715, resolution = 5) {
  getBoundingHexFromCoords(lat = lat, lon = lon, resolution = resolution)
}