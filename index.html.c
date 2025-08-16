<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ContextForge Studio Prototype</title>
    <link href="https://cdn.jsdelivr.net/npm/tailwindcss@2.2.19/dist/tailwind.min.css" rel="stylesheet">
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700&display=swap" rel="stylesheet">
    <style>
        body {
            font-family: 'Inter', sans-serif;
            background-color: #1F2937;
            color: #FFFFFF;
        }
        .button-hover {
            transition: background-color 0.2s, transform 0.1s;
        }
        .button-hover:hover {
            background-color: #0D9488;
            transform: translateY(-1px);
        }
        .slide-enter {
            opacity: 0;
            transform: translateX(100%);
        }
        .slide-enter-active {
            opacity: 1;
            transform: translateX(0);
            transition: opacity 0.3s ease, transform 0.3s ease;
        }
        .slide-exit {
            opacity: 1;
            transform: translateX(0);
        }
        .slide-exit-active {
            opacity: 0;
            transform: translateX(-100%);
            transition: opacity 0.3s ease, transform 0.3s ease;
        }
        .progress-step {
            width: 25%;
            height: 4px;
            background: #4B5563;
            transition: background 0.3s;
        }
        .progress-step-active {
            background: #0D9488;
        }
        .hidden {
            display: none;
        }
    </style>
</head>
<body class="min-h-screen p-4">
    <div class="max-w-md mx-auto">
        <!-- Progress Indicator -->
        <div class="flex justify-between mb-8 text-sm text-gray-400">
            <span class="progress-step" data-step="1"></span>
            <span class="progress-step" data-step="2"></span>
            <span class="progress-step" data-step="3"></span>
            <span class="progress-step" data-step="4"></span>
        </div>

        <!-- Onboarding Screens -->
        <div class="screens">
            <!-- Welcome Screen -->
            <div class="screen slide-enter slide-enter-active">
                <div class="bg-gray-800 p-6 rounded-xl shadow-md text-center">
                    <h1 class="text-2xl font-semibold mb-4">Welcome to ContextForge</h1>
                    <p class="text-gray-400 mb-6">Manage your contexts and prompts.</p>
                    <button class="w-full py-3 bg-teal-600 text-white rounded-lg button-hover">Next</button>
                </div>
            </div>

            <!-- Features Screen -->
            <div class="screen hidden">
                <div class="bg-gray-800 p-6 rounded-xl shadow-md text-center">
                    <h1 class="text-2xl font-semibold mb-4">Key Features</h1>
                    <p class="text-gray-400 mb-6">Context management, prompts, and chatbot integration.</p>
                    <button class="w-full py-3 bg-teal-600 text-white rounded-lg button-hover">Next</button>
                </div>
            </div>

            <!-- Context Setup Screen -->
            <div class="screen hidden">
                <div class="bg-gray-800 p-6 rounded-xl shadow-md text-center">
                    <h1 class="text-2xl font-semibold mb-4">Setup Your Context</h1>
                    <input id="contextName" type="text" placeholder="Context Name" class="w-full p-3 mb-4 bg-gray-700 rounded-lg text-white">
                    <select id="contextType" class="w-full p-3 mb-4 bg-gray-700 rounded-lg text-white">
                        <option value="">Select Type</option>
                        <option value="Factual">Factual</option>
                        <option value="Creative">Creative</option>
                    </select>
                    <button id="saveContextBtn" class="w-full py-3 bg-teal-600 text-white rounded-lg button-hover" disabled>Save</button>
                </div>
            </div>

            <!-- Chatbot Launch Screen -->
            <div class="screen hidden">
                <div class="bg-gray-800 p-6 rounded-xl shadow-md text-center">
                    <h1 class="text-2xl font-semibold mb-4">Launch Chatbot</h1>
                    <select id="chatbotSelect" class="w-full p-3 mb-4 bg-gray-700 rounded-lg text-white">
                        <option value="">Select Chatbot</option>
                        <option value="claude">Claude</option>
                        <option value="grok">Grok</option>
                        <option value="chatgpt">ChatGPT</option>
                    </select>
                    <button id="launchBtn" class="w-full py-3 bg-teal-600 text-white rounded-lg button-hover" disabled>Launch</button>
                </div>
            </div>

            <!-- Chat Interface (Post-Return) -->
            <div id="chatInterface" class="bg-gray-800 p-6 rounded-xl shadow-md text-center hidden">
                <h2 class="text-xl font-semibold mb-4">Chat Response</h2>
                <p id="responseText">Waiting for return...</p>
            </div>
        </div>

        <!-- Log for Debugging -->
        <div id="log" class="mt-6 p-4 bg-gray-800 rounded-lg text-gray-300"></div>
    </div>

    <script>
        const screens = document.querySelectorAll('.screen');
        let currentScreen = 0;
        const log = document.getElementById('log');
        const contextName = document.getElementById('contextName');
        const contextType = document.getElementById('contextType');
        const saveContextBtn = document.getElementById('saveContextBtn');
        const chatbotSelect = document.getElementById('chatbotSelect');
        const launchBtn = document.getElementById('launchBtn');
        const chatInterface = document.getElementById('chatInterface');
        const responseText = document.getElementById('responseText');

        function logMessage(message) {
            log.innerHTML += `${new Date().toLocaleTimeString()} - ${message}<br>`;
        }

        function navigate(step) {
            screens[currentScreen].classList.add('slide-exit');
            screens[currentScreen].classList.remove('slide-enter-active');
            setTimeout(() => {
                screens[currentScreen].classList.add('hidden');
                screens[currentScreen].classList.remove('slide-exit-active');
                currentScreen = step;
                screens[currentScreen].classList.remove('hidden', 'slide-exit');
                screens[currentScreen].classList.add('slide-enter-active');
                updateProgress();
                checkValidation();
            }, 300);
        }

        function updateProgress() {
            document.querySelectorAll('.progress-step').forEach((step, idx) => {
                step.classList.toggle('progress-step-active', idx <= currentScreen);
            });
        }

        function checkValidation() {
            saveContextBtn.disabled = !contextName.value.trim() || !contextType.value;
            contextName.addEventListener('input', () => {
                saveContextBtn.disabled = !contextName.value.trim() || !contextType.value;
            });
            contextType.addEventListener('change', () => {
                saveContextBtn.disabled = !contextName.value.trim() || !contextType.value;
            });
            launchBtn.disabled = !chatbotSelect.value;
            chatbotSelect.addEventListener('change', () => {
                launchBtn.disabled = !chatbotSelect.value;
            });
        }

        document.querySelectorAll('button').forEach((btn, index) => {
            btn.addEventListener('click', () => {
                if (index === 2 && (saveContextBtn.disabled || currentScreen !== 2)) return;
                if (index === 3 && launchBtn.disabled) return;
                if (index === 3) openChatbot(chatbotSelect.value);
                else navigate((currentScreen + 1) % 4); // Limit to 4 screens
            });
        });

        function openChatbot(chatbot) {
            let url, deepLink;
            switch (chatbot.toLowerCase()) {
                case 'claude':
                    deepLink = 'claude://';
                    url = 'https://claude.ai/login?returnto=contextforge://post-login';
                    break;
                case 'grok':
                    deepLink = 'x://';
                    url = 'https://x.ai/grok/login?returnto=contextforge://post-login';
                    break;
                case 'chatgpt':
                    deepLink = 'openai://';
                    url = 'https://chat.openai.com/login?returnto=contextforge://post-login';
                    break;
            }

            logMessage(`Attempting to open ${deepLink}`);
            const isCapacitorSim = true;
            if (isCapacitorSim) {
                setTimeout(() => {
                    logMessage(`Deep link ${deepLink} failed (simulated), falling back to ${url}`);
                    window.open(url, '_blank');
                }, 1000);
            } else {
                const canOpen = window.location.href.includes('android')
                    ? document.createElement('a').setAttribute('href', deepLink).click() || false
                    : window.open(deepLink, '_blank');
                if (!canOpen) {
                    logMessage(`Fallback to ${url}`);
                    window.open(url, '_blank');
                }
            }
        }

        // Simulate return mechanism
        window.addEventListener('hashchange', () => {
            if (window.location.hash === '#post-login') {
                logMessage('Returned to app via custom URL');
                screens.forEach(screen => screen.classList.add('hidden'));
                chatInterface.classList.remove('hidden');
                responseText.textContent = 'Simulated response from chatbot.';
            }
        });

        // Trigger return for testing
        function simulateReturn() {
            window.location.hash = 'post-login';
        }
        setTimeout(simulateReturn, 5000); // Simulate after 5 seconds
    </script>
</body>
</html>
