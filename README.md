# h3r

Uber's h3 geographical indexing library bindings for the R Programming Language.

## What?

- [h3](https://github.com/uber/h3)
- It is a way to index coordinate data into hexagonal bins
- So far, implemented methods help you
  - Convert coordinates to an index identifier (a unique ID for your hex)
  - Convert coordinates and index identifiers to a bounding hexagon
  
## Usage

### Prerequisites

- [h3](https://github.com/uber/h3). You'll need to follow the installation instructions.
- Linux or MacOS.

### Examples

```r
Houston <- list(lat = 29.7632836, lon = -95.3632715)

getIndexFromCoords(Houston$lat, Houston$lon, resolution = 5)
# [1] "85446cabfffffff"

hex <- getBoundingHexFromCoords(Houston$lat, Houston$lon, resolution = 5)
hexTib <- dplyr::bind_rows(hex)
hexTib
# # A tibble: 6 x 2
#        lat       lon
#      <dbl>     <dbl>
# 1 29.60453 -95.35630
# 2 29.67752 -95.28924
# 3 29.76643 -95.32811
# 4 29.78236 -95.43420
# 5 29.70931 -95.50129
# 6 29.62039 -95.46226

# We can plot the hex in leaflet
library(leaflet)
library(magrittr)
leaflet() %>% addTiles() %>% addPolygons(lng = hexTib$lon, lat = hexTib$lat)

# We can plot the hex in ggmap
library(ggmap)
ggmap(get_googlemap()) + geom_polygon(data = hexTib, mapping = aes(x = lon, y = lat))
```
