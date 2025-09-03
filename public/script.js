// Connect to the server using Socket.IO
const socket = io();

let realTurbidityReceived = false;

// Listen for real sensor data from the server
socket.on('sensorData', (data) => {
    // Try to extract the numeric turbidity value from the data string
    let value = null;
    // If Arduino sends "Turbidity: 641", extract 641
    const match = data.match(/Turbidity:\s*(\d+)/i);
    if (match) {
        value = parseInt(match[1], 10);
    } else if (!isNaN(parseInt(data, 10))) {
        value = parseInt(data, 10);
    }
    console.log('Received sensorData:', data, 'Parsed value:', value);
    if (typeof value === 'number' && !isNaN(value)) {
        realTurbidityReceived = true;
        document.getElementById('turbidity-value').textContent = value;
        if (window.turbidityMonitor) {
            window.turbidityMonitor.updateWaterQualityStatus(value);
            window.turbidityMonitor.updateChart(value);
        }
    }
});

class TurbidityMonitor {
    constructor() {
        this.chart = null;
        this.init();
    }

    init() {
        this.updateDateTime();
        this.initChart();
        // Start with initial simulated data for turbidity
        this.startDataSimulation();

        // Immediately fetch and display real temperature data
        this.getRealTemperature();

        this.updateInterval = setInterval(() => {
            this.updateDateTime();
            // Only simulate if no real data has been received
            if (!realTurbidityReceived) {
                this.simulateNewData();
            }
            // Fetch and update real temperature data
            this.getRealTemperature();
        }, 30000); // Update every 30 seconds
    }

    async getRealTemperature() {
        const temperatureElement = document.getElementById('temperature-value');
        try {
            const location = await getUserLocation();
            const temperature = await getTemperature(location.lat, location.lon);

            if (temperature !== null) {
                temperatureElement.textContent = temperature.toFixed(1);
            } else {
                temperatureElement.textContent = "N/A";
            }
        } catch (error) {
            console.error("An error occurred while fetching temperature:", error);
            temperatureElement.textContent = "N/A";
        }
    }

    updateDateTime() {
        const now = new Date();
        const timeString = now.toLocaleTimeString('en-US', { 
            hour: '2-digit', 
            minute: '2-digit' 
        });
        document.getElementById('last-update').textContent = timeString;
    }

    initChart() {
        const ctx = document.getElementById('turbidityChart').getContext('2d');
        
        // Generate sample data for the last 24 hours
        const data = this.generateSampleData();
        
        this.chart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: data.labels,
                datasets: [{
                    label: 'Turbidity (NTU)',
                    data: data.values,
                    borderColor: '#2563eb',
                    backgroundColor: 'rgba(37, 99, 235, 0.1)',
                    borderWidth: 2,
                    fill: true,
                    tension: 0.4,
                    pointRadius: 0,
                    pointHoverRadius: 6
                }]
            },
            options: {
                responsive: true,
                maintainAspectRatio: false,
                plugins: {
                    legend: {
                        display: false
                    }
                },
                scales: {
                    x: {
                        grid: {
                            display: false
                        },
                        ticks: {
                            maxTicksLimit: 6
                        }
                    },
                    y: {
                        beginAtZero: true,
                        max: 5,
                        grid: {
                            color: '#e2e8f0'
                        },
                        ticks: {
                            stepSize: 1
                        }
                    }
                },
                interaction: {
                    intersect: false,
                    mode: 'index'
                }
            }
        });
    }

    generateSampleData() {
        const now = new Date();
        const labels = [];
        const values = [];
        
        for (let i = 23; i >= 0; i--) {
            const time = new Date(now - i * 60 * 60 * 1000);
            labels.push(time.toLocaleTimeString('en-US', { 
                hour: '2-digit', 
                minute: '2-digit' 
            }));
            
            // Generate realistic turbidity values (0-5 NTU)
            const baseValue = 2.0;
            const variation = (Math.sin(i / 4) + Math.random() - 0.5) * 0.8;
            values.push(Math.max(0, Math.min(5, baseValue + variation)));
        }
        
        return { labels, values };
    }

    simulateNewData() {
        // Simulate real-time data updates for turbidity only
        const turbidityElement = document.getElementById('turbidity-value');
        
        // Generate new turbidity value with small variations
        const currentTurbidity = parseFloat(turbidityElement.textContent);
        const newTurbidity = Math.max(0, Math.min(5, 
            currentTurbidity + (Math.random() - 0.5) * 0.2
        ));
        
        // Update display value
        turbidityElement.textContent = newTurbidity.toFixed(1);
        
        // Update status based on turbidity
        this.updateWaterQualityStatus(newTurbidity);
        
        // Update chart
        this.updateChart(newTurbidity);
    }

    updateWaterQualityStatus(turbidity) {
        const statusElement = document.querySelector('.metric-status');
        const statusIndicator = document.querySelector('.status-indicator');
        
        let status, className;
        
        if (turbidity <= 1) {
            status = 'Excellent Quality';
            className = 'good';
        } else if (turbidity <= 4) {
            status = 'Good Quality';
            className = 'good';
        } else if (turbidity <= 10) {
            status = 'Fair Quality';
            className = 'warning';
        } else {
            status = 'Poor Quality';
            className = 'danger';
        }
        
        statusElement.textContent = status;
        statusIndicator.className = `status-indicator ${className}`;
    }

    updateChart(newValue) {
        if (this.chart) {
            const data = this.chart.data;
            const now = new Date();
            
            // Remove oldest data point
            data.labels.shift();
            data.datasets[0].data.shift();
            
            // Add new data point
            data.labels.push(now.toLocaleTimeString('en-US', { 
                hour: '2-digit', 
                minute: '2-digit' 
            }));
            data.datasets[0].data.push(newValue);
            
            this.chart.update('none');
        }
    }

    startDataSimulation() {
        // Initial realistic values
        document.getElementById('turbidity-value').textContent = '2.3';
        // document.getElementById('temperature-value').textContent = '22.5'; // No longer needed
        
        this.updateWaterQualityStatus(2.3);
    }
}

// Smooth scrolling for navigation links
document.querySelectorAll('a[href^="#"]').forEach(anchor => {
    anchor.addEventListener('click', function (e) {
        e.preventDefault();
        const target = document.querySelector(this.getAttribute('href'));
        if (target) {
            target.scrollIntoView({
                behavior: 'smooth',
                block: 'start'
            });
        }
    });
});

// Initialize the monitoring system when the page loads
document.addEventListener('DOMContentLoaded', () => {
    window.turbidityMonitor = new TurbidityMonitor(); // Generated by Copilot
});

// Add some interactive features
document.addEventListener('click', (e) => {
    if (e.target.closest('.card')) {
        e.target.closest('.card').style.transform = 'scale(0.98)';
        setTimeout(() => {
            e.target.closest('.card').style.transform = '';
        }, 150);
    }
});

// Function to get the user's current location (latitude and longitude)
function getUserLocation() {
  return new Promise((resolve, reject) => {
    if (navigator.geolocation) {
      navigator.geolocation.getCurrentPosition(
        (position) => {
          resolve({
            lat: position.coords.latitude,
            lon: position.coords.longitude
          });
        },
        (error) => {
          reject(new Error("Geolocation not available or permission denied."));
        }
      );
    } else {
      reject(new Error("Geolocation is not supported by your browser."));
    }
  });
}

// Function to fetch weather data from WeatherAPI.com, now using the .env variables
async function getTemperature(lat, lon) {
  const url = `${WEATHER_API_URL}?key=${WEATHER_API_KEY}&q=${lat},${lon}`;

  try {
    const response = await fetch(url);
    if (!response.ok) {
      throw new Error(`HTTP error! status: ${response.status}`);
    }
    const data = await response.json();
    return data.current.temp_c; // Returns temperature in Celsius
  } catch (error) {
    console.error("Could not fetch temperature data:", error);
    return null;
  }
}