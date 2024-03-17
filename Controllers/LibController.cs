using System;
using Facecore.Models;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;

namespace Facecore.Controllers
{
    [Route("api/[controller]/[action]")]
    [ApiController]
    public class LibController : ControllerBase
    {
        private readonly ILogger<UserController> _logger;
        private readonly FacecoreContext _context;

        public LibController(ILogger<UserController> logger, FacecoreContext context)
        {
            _logger = logger;
            _context = context;
        }

        public object getAllLib()
        {
            try
            {
                var lib = _context.lib;
                return lib;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return StatusCode(404);
            }
            
        }

    }
}
