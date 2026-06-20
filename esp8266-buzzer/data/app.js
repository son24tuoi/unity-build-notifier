function scanWifi() {
    const select = document.getElementById("ssidSelect");
    const message = document.getElementById("message");

    message.textContent = "";

    select.innerHTML = "<option>Scanning...</option>";

    fetch("/scan")
        .then(response => response.json())
        .then(data => {
            select.innerHTML = "";

            if (data.length === 0) {
                select.innerHTML = "<option>No wifi found</option>";

                return;
            }

            data.forEach(wifi => {
                const option = document.createElement("option");

                option.value = wifi.ssid;
                option.textContent = wifi.ssid + " (" + wifi.rssi + " dBm)" + (wifi.secure ? " 🔒" : "");

                select.appendChild(option);
            });

        })
        .catch(error => {
            message.textContent = `Error: ${error.message}`;

            console.log(error);
        });
}

function saveWifi() {
    const ssid = document.getElementById("ssidSelect").value;
    const password = document.getElementById("passwordInput").value;
    const message = document.getElementById("message");

    if (!ssid) {
        message.textContent = "Please select a wifi";

        return;
    }

    const formData = new URLSearchParams();
    formData.append("ssid", ssid);
    formData.append("password", password);

    fetch("/save",
        {
            method: "POST",
            headers:
            {
                "Content-Type":
                    "application/x-www-form-urlencoded"
            },
            body: formData.toString()
        })
        .then(response => response.text())
        .then(text => {
            message.textContent = text;
        })
        .catch(error => {
            message.textContent = "Error: " + error.message;

            console.log(error);
        });
}

function saveWifi() {
    const ssid = document.getElementById("ssidSelect").value;
    const password = document.getElementById("passwordInput").value;
    const message = document.getElementById("message");

    if (!ssid) {
        message.textContent = "Please select a WiFi";
        return;
    }

    message.textContent = "Checking WiFi connection...";

    const formData = new URLSearchParams();

    formData.append("ssid", ssid);
    formData.append("password", password);

    fetch("/save",
        {
            method: "POST",
            headers:
            {
                "Content-Type": "application/x-www-form-urlencoded"
            },
            body: formData.toString()
        })
        .then(response => response.json())
        .then(data => {
            message.textContent = data.message;
        })
        .catch(error => {
            message.textContent = "Error: " + error.message;
            console.log(error);
        });
}

window.onload = function () {
    scanWifi();
};

function resetWifi() {
    const message = document.getElementById("message");

    if (!confirm("Reset WiFi settings and restart the device?")) {
        return;
    }

    message.textContent = "Resetting WiFi...";

    fetch("/clear_data", { method: "POST" })
        .then(response => response.json())
        .then(data => {
            message.textContent = data.message || "Device will restart.";
        })
        .catch(error => {
            message.textContent = "Error: " + error.message;
            console.log(error);
        });
}

function restartDevice() {
    const message = document.getElementById("message");

    if (!confirm("Restart the device now?")) {
        return;
    }

    message.textContent = "Restarting device...";

    fetch("/restart", { method: "POST" })
        .then(response => response.json())
        .then(data => {
            message.textContent = data.message || "Restarting...";
        })
        .catch(error => {
            message.textContent = "Error: " + error.message;
            console.log(error);
        });
}