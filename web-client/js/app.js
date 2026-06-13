const ipInput = document.getElementById("ip");

window.onload = () => {

    const savedIP = localStorage.getItem("esp_ip");

    if (savedIP) {
        ipInput.value = savedIP;
    }
};

function saveIP() {
    localStorage.setItem(
        "esp_ip",
        ipInput.value.trim()
    );
}

function getBaseUrl() {

    saveIP();

    return `http://${ipInput.value.trim()}/command`;
}

function addLog(message) {

    const now = new Date().toLocaleTimeString();

    const log = document.getElementById("log");

    log.innerHTML += `[${now}] ${message}<br>`;

    log.scrollTop = log.scrollHeight;
}

function clearLog() {
    document.getElementById("log").innerHTML = "";
}

function showToast(message, type = "info") {

    const toast = document.getElementById("toast");

    toast.className = "";

    toast.classList.add(`toast-${type}`);

    toast.innerText = message;

    toast.style.display = "block";

    clearTimeout(window.toastTimer);

    window.toastTimer = setTimeout(() => {

        toast.style.display = "none";

    }, 1500);
}

async function testConnection() {

    try {

        const url = `${getBaseUrl()}?command`;

        console.log("URL:", url);

        const response = await fetch(url);

        console.log("Status:", response.status);

        const text = await response.text();

        console.log("Response:", text);

        if (text.includes("File Not Found")) {

            // setStatus("🟢 ESP8266 Connected");

            addLog("Connection OK");

            showToast(
                "✅ ESP8266 Connected",
                "success"
            );
        }
        else {
            // setStatus("🔴 Disconnected");

            addLog("Connection Failed");

            showToast(
                "❌ Disconnected",
                "error"
            );
        }
    }
    catch (e) {

        console.error(e);

        addLog(`Connection Failed: ${e.message}`);

        showToast(
            `❌ ${e.message}`,
            "error"
        );
    }
}

async function sendRequest(command) {

    try {

        const url = `${getBaseUrl()}?command=${encodeURIComponent(command)}`;

        const response = await fetch(
            url,
            {
                method: "POST"
            });

        const result = await response.text();

        addLog(`SEND -> ${command}`);
        addLog(`RESP <- ${result}`);

        showToast(
            `📨 Sent: ${command}`,
            "info"
        );
    }
    catch (e) {

        addLog(
            `ERROR -> ${e.message}`
        );

        showToast(
            "❌ Không kết nối được ESP8266",
            "error"
        );
    }
}

function sendCommand(code) {
    sendRequest(code);
}

function sendCustom(code) {

    const input = document.getElementById("customText");

    const message = input.value.trim();

    if (!message) {

        showToast(
            "⚠ Chưa nhập nội dung",
            "error"
        );

        return;
    }

    sendRequest(
        (code === 100) ?
            message :
            `${code}|${message}`
    );
}

// function setStatus(message) {

//     const status =
//         document.getElementById("status");

//     status.innerText = message;
// }