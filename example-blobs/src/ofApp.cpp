// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#include "ofApp.h"


void ofApp::setup()
{
    ofSetFrameRate(30);
    ofEnableAlphaBlending();

    /// Example Database
    std::string mbTilesFilename = ofToDataPath("blank-1-3.mbtiles", true);

    try
    {
        // Open a database file in readonly mode
        SQLite::Database db(mbTilesFilename);  // SQLITE_OPEN_READONLY

        int zoomLevel = 1;
        int tileColumn = 0;
        int tileRow = 0;

        // We use string stream to create a long query string, but one
        // could just as easily use a string with the += operator.  It's style.
        std::stringstream ss;

        ss << "SELECT zoom_level, tile_column, tile_row, tile_data ";
        ss << "FROM tiles ";
        ss << "WHERE zoom_level = ? ";
        ss << "AND tile_column = ? ";
        ss << "AND tile_row = ?";

        // Create the query with the database.
        SQLite::Statement query(db, ss.str());

        query.bind(1, zoomLevel);  // Bind the zoom level to the first ? in the query.
        query.bind(2, tileColumn); // Bind the tile column to the second ? in the query.
        query.bind(3, tileRow); // Bind the tile row to the third ? in the query.

        // Loop to execute the query step by step, to get one a row of results at a time
        while (query.executeStep())
        {
            SQLite::Column colBlob = query.getColumn(3); // We know that column 3 is the image bytes
            const void* blob = colBlob.getBlob(); // Get a pointer to the bytes.
            std::size_t size = colBlob.getBytes(); // Get the number of bytes.
            ofBuffer buffer(static_cast<const char*>(blob), size); // Create a buffer with those bytes.

            img.loadImage(buffer); // Load the image from the buffer.
        }

    }
    catch (std::exception& e)
    {
        std::cout << "SQLite exception: " << e.what() << std::endl;
    }
}


void ofApp::draw()
{
    ofBackgroundGradient(ofColor::white, ofColor::black);

    // Draw the tile image.
    img.draw(0, 0);
}
