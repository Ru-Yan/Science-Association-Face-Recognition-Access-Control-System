using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using System;

namespace Facecore.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class TestPointController : ControllerBase
    {

        private readonly ILogger<TestPointController> _logger;

        public TestPointController(ILogger<TestPointController> logger)
        {
            _logger = logger;
        }

        public object Home()
        {
            return new
            {
                Test = "Hello!",
                TimeSpan = DateTime.Now
            };
        }

    }
}
