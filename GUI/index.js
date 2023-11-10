let file_contents = "";
let image_name = "";

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

    }
}

const input = document.querySelector("input");
input.addEventListener("change", function () {
    if (this.files && this.files[0]) {
        image_name = this.files[0].name.split(".")[0];
        console.log(this.files[0]);
        render_image(URL.createObjectURL(this.files[0]));

    }
});

const download = document.getElementById("download");
download.addEventListener("click", function () {
    var url = "data:text/plain;charset=utf-8," + encodeURIComponent(file_contents);
    var element = document.createElement('a');
    element.setAttribute("href", `${url}`);
    element.setAttribute('download', `tiny_pov_${image_name}.txt`);
    element.style.display = 'none';
    document.body.appendChild(element);
    element.click();
    document.body.removeChild(element);
    var post_upload = document.getElementById('post_download');
    post_upload.style.display = "block";
});