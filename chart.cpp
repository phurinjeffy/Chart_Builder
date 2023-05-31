#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

using namespace std;

class Chart {
public:
    Chart(string filename, string title, int num_values, int values[], string colors[], string names[], string line_color="black") : 
        filename_(filename), title_(title), num_values_(num_values), values_(values), colors_(colors), names_(names), line_color_(line_color) {}

    virtual void generate() = 0;

protected:
    string filename_;
    string title_;
    int num_values_;
    int* values_;
    string* colors_;
    string* names_;
    string line_color_;
};

class BarChart : public Chart {
public:
    BarChart(string filename, string title, int num_values, int values[], string colors[], string names[]) :
        Chart(filename, title, num_values, values, colors, names) {}

    void generate() {
        ofstream outfile(filename_ + ".html");

        // Title
        outfile << "<html>\n<head>\n<title>" << title_ << "</title>\n</head>\n<body>\n";
        outfile << "<h2 style=\"text-align:center;\">" << title_ << "</h2>\n";

        // Bar Chart
        outfile << "<div style=\"margin: 0 auto; width: " << 60 * num_values_ << "px;\">\n";
        for (int i = 0; i < num_values_; i++) {
            outfile << "<div style=\"position: relative; background-color: " << colors_[i] << "; height: " << values_[i] << "px; width: 50px; display: inline-block; margin: 5px 0;\">\n";
            outfile << "<span style=\"position: absolute; top: 0; left: 0; right: 0; bottom: 0; text-align: center; line-height: " << values_[i] << "px;\">" << values_[i] << "</span>\n";
            outfile << "</div>\n";
        }
        outfile << "</div>\n";

        // Legends
        outfile << "<div style=\"margin: 0 auto; width: " << 60 * num_values_ << "px;\">\n";
        for (int i = 0; i < num_values_; i++) {
            outfile << "<div style=\"width: 50px; display: inline-block; text-align: center;\">" << names_[i] << "</div>\n";
        }
        outfile << "</div>\n";

        outfile << "</body>\n</html>";

        outfile.close();
        cout << "Bar chart generated in " << filename_ << ".html" << endl;
    }
};

class PieChart : public Chart {
public:
    PieChart(string filename, string title, int num_values, int values[], string colors[], string names[]) :
         Chart(filename, title, num_values, values, colors, names) {}

    void write(ostream& os){
        os << "HELLO";
    }

    void generate() {
        ofstream outfile(filename_ + ".html");

        // Title
        outfile << "<html>\n<head>\n<title>" << title_ << "</title>\n</head>\n<body>\n";
        outfile << "<h2 style=\"text-align:center;\">" << title_ << "</h2>\n";

        // Pie Chart
        outfile << "<div style=\"margin: 0 auto; display: flex; justify-content: center; px;\">\n";
        outfile << "<svg width=\"500\" height=\"500\">\n";
        double total_value = 0.0;
        for (int i = 0; i < num_values_; i++) {
            total_value += values_[i];
        }
        double angle = -90.0; // start at 12 o'clock
        for (int i = 0; i < num_values_; i++) {
            double percentage = values_[i] / total_value;
            double slice_angle = percentage * 360.0;
            outfile << "<path fill=\"" << colors_[i] << "\" d=\"M 250,250 " 
                    << 250 + 250*cos(angle*M_PI/180.0) << "," << 250 + 250*sin(angle*M_PI/180.0) 
                    << " A 250,250 0 " << (slice_angle > 180 ? 1 : 0) << ",1 " 
                    << 250 + 250*cos((angle+slice_angle)*M_PI/180.0) << "," << 250 + 250*sin((angle+slice_angle)*M_PI/180.0) 
                    << " Z\"/>\n";
            angle += slice_angle;
        }
        outfile << "</svg>\n";

        // Legends
        outfile << "<div style=\"display: flex; justify-content: center; margin-top: 20px;\">\n";
        for (int i = 0; i < num_values_; i++) {
            outfile << "<div style=\"display: flex; align-items: center; margin-right: 20px;\">\n";
            outfile << "<div style=\"width: 10px; height: 10px; background-color: " << colors_[i] << "; margin-right: 5px;\"></div>\n";
            outfile << "<div>" << names_[i] << "</div>\n";
            outfile << "</div>\n";
        }
        outfile << "</div>\n";

        outfile << "</body>\n</html>";

        outfile.close();
        cout << "Pie chart generated in " << filename_ << ".html" << endl;
    }
};

class LineChart : public Chart {
public:
    LineChart(string filename, string title, int num_values, int values[], string colors[], string names[], string line_color) :
         Chart(filename, title, num_values, values, colors, names, line_color) {}

    void generate() {
        ofstream outfile(filename_ + ".html");

        // Title
        outfile << "<html>\n<head>\n<title>" << title_ << "</title>\n</head>\n<body>\n";
        outfile << "<h2 style=\"text-align:center;\">" << title_ << "</h2>\n";

        double y_min = values_[0], y_max = values_[0];
        for (int i = 1; i < num_values_; i++) {
            if (values_[i] < y_min) y_min = values_[i];
            if (values_[i] > y_max) y_max = values_[i];
        }

        int canvas_width = 600, canvas_height = 400;
        int margin_top = 50, margin_right = 50, margin_bottom = 50, margin_left = 80;

        double y_scale = (canvas_height - margin_top - margin_bottom) / (y_max - y_min);

        // Graph
        outfile << "<div style=\"margin: 0 auto; display: flex; justify-content: center; px;\">\n";
        outfile << "<svg width=\"" << canvas_width << "\" height=\"" << canvas_height << "\">\n";
        outfile << "<rect x=\"" << margin_left << "\" y=\"" << margin_top << "\" width=\"" 
                << canvas_width - margin_left - margin_right << "\" height=\"" 
                << canvas_height - margin_top - margin_bottom << "\" fill=\"none\" stroke=\"black\"/>\n"; // draw the axes box
        
        // Y-Axis
        for (int i = 0; i <= 10; i++) {
            double y_value = y_min + (i * (y_max - y_min) / 10);
            double y_pos = canvas_height - margin_bottom - (y_value - y_min) * y_scale;
            outfile << "<text x=\"" << (margin_left - 5) << "\" y=\"" << y_pos << "\" style=\"text-anchor: end; dominant-baseline: middle;\">" << y_value << "</text>\n";
            outfile << "<line x1=\"" << margin_left << "\" y1=\"" << y_pos << "\" x2=\"" << (canvas_width - margin_right) << "\" y2=\"" << y_pos << "\" stroke-dasharray=\"5,5\" stroke=\"#ccc\"/>\n";
        }

        // X-Axis
        for (int i = 0; i < num_values_; i++) {
            double x = margin_left + i * ((canvas_width - margin_left - margin_right) / (num_values_ - 1));
            outfile << "<text x=\"" << x << "\" y=\"" << (canvas_height - margin_bottom + 20) << "\" style=\"text-anchor: middle; dominant-baseline: middle;\">" << names_[i] << "</text>\n";
        }
        
        // Line
        outfile << "<path fill=\"none\" stroke=\"" << line_color_ << "\" d=\""; // start the line path
        for (int i = 0; i < num_values_; i++) {
            double x = margin_left + i * ((canvas_width - margin_left - margin_right) / (num_values_ - 1));
            double y = canvas_height - margin_bottom - (values_[i] - y_min) * y_scale;
            if (i == 0) outfile << "M " << x << " " << y; // move to the first point
            else outfile << " L " << x << " " << y; // draw a line to the next point
        }
        outfile << "\"/>\n"; // end the line path
        
        // Draw the circle for point
        for (int i = 0; i < num_values_; i++) {
            outfile << endl << "<g fill=" << colors_[i] << ">" << endl;
            double x = margin_left + i * ((canvas_width - margin_left - margin_right) / (num_values_ - 1));
            double y = canvas_height - margin_bottom - (values_[i] - y_min) * y_scale;
            outfile << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"2\" stroke=\"" << colors_[i] << "\" stroke-width=\"2\" fill=\"" << colors_[i] << "\"/>" << endl;
        }

        outfile << "</g>" << endl;
        outfile << "</svg>\n";

        outfile << "</body>\n</html>";

        outfile.close();
        cout << "Line Chart generated in " << filename_ << ".html" << endl;
    }
};

using Chart_ptr = unique_ptr<Chart>;

int main() {
    string chart_type;
    cout << "Chart type: ";
    cin >> chart_type;

    int num_values;
    cout << "Number of values: ";
    cin >> num_values;

    int values[num_values];
    string names[num_values];
    string colors[num_values];
    string color;
    if(chart_type == "line"){
        cout << "Enter line color: ";
        cin >> color;
    }
    for (int i = 0; i < num_values; i++) {
        cout << "Enter Name" << i + 1 << ": ";
        cin >> names[i];
        cout << "Enter " << names[i] << " value: ";
        cin >> values[i];
        cout << "Enter " << names[i] << " color: ";
        cin >> colors[i];
    }

    string filename;
    cout << "Filename: ";
    cin >> filename;

    vector<Chart_ptr> graph;
    int v0[] = {100,200,300};
    string c0[] = {"red", "green", "blue"};
    string n0[] = {"Jeff", "Yok", "Ogee"};

    BarChart b0("bar", "BAR", 3, v0, c0, n0);
    graph.push_back(make_unique<BarChart>(b0));
    b0.generate();

    PieChart p0("pie", "PIE", 3, v0, c0, n0);
    graph.push_back(make_unique<PieChart>(p0));
    p0.generate();

    LineChart l0("line", "LINE", 3, v0, c0, n0, "black");
    graph.push_back(make_unique<LineChart>(l0));
    l0.generate();

    // for(auto i = 0u; i < graph.size(); i++){
    //     delete[] graph[i];
    // }

    if(chart_type == "bar"){
        BarChart bar1(filename, "Bar Chart", num_values, values, colors, names);
        bar1.generate();
    }
    else if(chart_type == "pie"){
        PieChart pie1(filename, "Pie Chart", num_values, values, colors, names);
        pie1.generate();
    }
    else if(chart_type == "line"){
        LineChart line1(filename, "Line Chart", num_values, values, colors, names, color);
        line1.generate();
    }

    return 0;
}
