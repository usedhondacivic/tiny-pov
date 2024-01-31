/**
 * HELPERS
 */
// https://gist.github.com/xposedbones/75ebaef3c10060a3ee3b246166caab56
const constrain = (val, min, max) => (val < min ? min : (val > max ? max : val))
const map = (value, x1, y1, x2, y2) => (value - x1) * (y2 - x2) / (y1 - x1) + x2;

// IMAGE UTILS
let image_data = [];
let image_width = null;
let image_height = null;

// Get the image color (R, G, B) * A 
function get_color_at_point(svg_point, bounding_box){
    let x = Math.floor(map(svg_point.x, bounding_box.x, bounding_box.x + bounding_box.width, 0, image_width - 1));
    let y = Math.floor(map(svg_point.y, bounding_box.y, bounding_box.y + bounding_box.height, 0, image_height - 1));
    let color = image_data.slice((y * image_width + x) * 4, (y * image_width + x) *4 + 3);
    let a = image_data[(y * image_width + x + 3) * 4];
    for(var i=0; i<a.length; i++) {
        color[i] *= a / 255;
    }
    return color;
}

// D3 VIS

let graph = {
    divisions_per_rot: 150,
    num_pixels: 64,
    size: 10,
    radius: 4,
    pixel_data: [],
    sampled_points: [],
    bounding_box: null,
    init() {
        this.svg = d3.select("#preview_svg")
        .attr("width", "100vw")
        .attr("viewBox", [-this.size / 2, -this.size / 2, this.size, this.size]);

        let group = this.svg.append("g");

        // Fill with grey
        for(var i = 0; i < this.num_pixels * this.divisions_per_rot; i++){
            this.pixel_data.push({slice: Math.floor(i / this.num_pixels), color: [50, 50, 50]});
        }
        this.update();

        this.bounding_box = group.node().getBBox();

        // Handle zooming and panning
        let zoom = d3.zoom().on('zoom', handleZoom);
        function handleZoom(e) {
            d3.selectAll('svg > g')
                .attr('transform', e.transform);
        }
        d3.select('svg').call(zoom);
    },

    update(){
        this.svg.select("g")
        .selectAll("path")
        .data(this.pixel_data, (d, i) => {return i})
        .join(
            enter => {
                 return enter.append("path")
                .attr("d", (d, i) => {
                    let buffer = 0.0;
                    let inner_rad = (((i % this.num_pixels) * 2 - this.num_pixels) + 1) / 2 * (this.radius / (this.num_pixels/2));
                    let sign_angle_offset = 0;
                    let slice_size = ((2 * 3.141592653) / this.divisions_per_rot);
                    let slice_height = this.radius / (this.num_pixels) + buffer;
                    let slice_num = Math.floor(i / this.num_pixels);

                    let arc = {};
                    
                    if(inner_rad > 0){
                        arc = {
                            innerRadius: Math.abs(inner_rad) - buffer,
                            outerRadius: Math.abs(inner_rad) + slice_height + buffer,
                            startAngle: sign_angle_offset + slice_num * slice_size - buffer,
                            endAngle: sign_angle_offset + (slice_num + 1) * slice_size + buffer,
                        };
                    }else{
                        sign_angle_offset = Math.PI;
                        arc = {
                            innerRadius: Math.abs(inner_rad) - slice_height - buffer,
                            outerRadius: Math.abs(inner_rad) + buffer,
                            startAngle: sign_angle_offset + slice_num * slice_size - buffer,
                            endAngle: sign_angle_offset + (slice_num + 1) * slice_size + buffer,
                        };
                    }

                    this.log_point(arc);

                    return d3.arc()(arc);
                })
                .style("fill", d => {return `rgb(${d.color[0]}, ${d.color[1]}, ${d.color[2]})`});
            },
            update => {
                return update
                .transition()
                    .delay(function(d){ return 5 * d.slice; }) 
                    .duration(300)
                    .style("fill", d => {return `rgb(${d.color[0]}, ${d.color[1]}, ${d.color[2]})`});
            },
            exit => {
                return exit.remove();
            }
        );
    },

    log_point(arc){
        let angle = (arc.startAngle + arc.endAngle) / 2;
        let radius = (arc.innerRadius + arc.outerRadius) / 2;
        let x = Math.cos(angle - Math.PI / 2) * radius;
        let y = Math.sin(angle - Math.PI / 2) * radius;
        this.sampled_points.push({x: x, y: y});
    }
}

document.addEventListener('DOMContentLoaded', function(){
    graph.init();
})

// DOM / UI MANIPULATION
function render_image(url) {
    var post_upload = document.getElementById('post_upload');
    post_upload.style.display = "block";
    var img = document.getElementById('snapshot');
    img.style.display = "inline-block";
    img.onload = () => {
        URL.revokeObjectURL(img.src);
    }
    img.src = url;
    img.onload = function () {
        const canvas = document.createElement('canvas');
        const ctx = canvas.getContext('2d');

        // Bunch of sloppy logic to resize the image / canvas to play nice with the frame bounding box.
        // The image is centered and scaled to fill the frame
        const max_res = 600;
        let frame_ar = graph.bounding_box.width / graph.bounding_box.height;
        let img_ar = img.width / img.height;
        canvas.width = frame_ar >= 1 ? max_res : max_res * frame_ar;
        canvas.height = frame_ar < 1 ? max_res : max_res / frame_ar;
        let w = frame_ar >= img_ar ? canvas.width : canvas.height * img_ar;
        let h = frame_ar < img_ar ? canvas.height : canvas.width / img_ar;
        //ctx.filter = 'blur(1px)';
        ctx.drawImage(img, - (w - canvas.width) / 2, - (h - canvas.height) / 2, w, h);
        image_width = canvas.width;
        image_height = canvas.height;
        image_data = ctx.getImageData(0, 0, image_width, image_height).data;

        graph.pixel_data = [];
        for(var i = 0; i < graph.num_pixels * graph.divisions_per_rot; i++){
           let c = get_color_at_point(graph.sampled_points[i], graph.bounding_box);
            graph.pixel_data.push({slice: Math.floor(i / graph.num_pixels), color: c});
        }

        graph.update();
    }
}

let image_name = "";

const input = document.querySelector("input");
input.addEventListener("change", function () {
    if (this.files && this.files[0]) {
        image_name = this.files[0].name.split(".")[0];
        render_image(URL.createObjectURL(this.files[0]));
    }
});

const download = document.getElementById("download");
download.addEventListener("click", function () {
    let norm_image = graph.pixel_data.map(d => [].slice.call(d.color.reverse())); // [].slice.call -> convert uint8 array to normal js array
	let flat_image = norm_image.flatMap(d => [225, ...d]);
	let converted_arr = new Uint8Array(flat_image);
	console.log(converted_arr);
	var file = new File([converted_arr], {type: "application/octet-stream"});
    var element = document.createElement('a');
    element.setAttribute("href", `${URL.createObjectURL(file)}`);
    element.setAttribute('download', `tiny_pov_${image_name.replace(/[^a-zA-Z]/g, "").toUpperCase()}.pov`);
    element.style.display = 'none';
    document.body.appendChild(element);
    element.click();
    document.body.removeChild(element);
    var post_upload = document.getElementById('post_download');
    post_upload.style.display = "block";
});
