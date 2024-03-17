using System;
using System.Linq;
using Facecore.Models;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;

namespace Facecore.Controllers
{
    [Route("api/[controller]/[action]")]
    [ApiController]
    public class ClockInController : ControllerBase
    {
        private readonly ILogger<UserController> _logger;
        private readonly FacecoreContext _context;

        public ClockInController(ILogger<UserController> logger, FacecoreContext context)
        {
            _logger = logger;
            _context = context;
        }

        public object clockIn()
        {
            try
            {
                var acti = _context.acti;
                return acti;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return StatusCode(404);
            }
            
        }

    }
}
