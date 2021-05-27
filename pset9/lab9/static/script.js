
document.querySelector("#message").style.display = "none";

document.addEventListener('DOMContentLoaded', function() {

    document.querySelector("#submit").onsubmit = function() {
        document.querySelector("#message").style.display = "block";
    };

});

